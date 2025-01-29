/*
 * bubbl
 * Copyright (C) 2025  Raghuram Subramani <raghus2247@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Bitmap Based Allocation:
 * https://web.archive.org/web/20190316115948/http://www.brokenthorn.com/Resources/OSDev17.html
 */

/* TODO: Stack based allocation? */

#include <kernel/halt.h>
#include <kernel/spinlock.h>
#include <libk/stdio.h>
#include <mm/memory_map.h>
#include <mm/physical_mm.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t block_count = 0;
uint32_t total_free_blocks = 0;
uint32_t memory_map[MAX_BLOCKS / BITMAP_ENTRY_SIZE];

atomic_flag memory_map_lock;

ALWAYS_INLINE static void
physical_mm_log_memory_map(free_memory_regions_t *free_memory_regions)
{
  printk("\nphysical_mm", "memory_map is at 0x%x", memory_map);
  printk("\nphysical_mm", "Free Memory Regions:");
  for (int i = 0; i < free_memory_regions->n_regions; i++)
    printk("physical_mm",
           "start: 0x%.08x | length: 0x%.08x",
           free_memory_regions->region_list[i]->addr_low,
           free_memory_regions->region_list[i]->len_low);

  printk("\nphysical_mm", "Kernel region:");
  printk("physical_mm", "Start: 0x%x", &kernel_start);
  printk("physical_mm", "End:   0x%x", &kernel_end);
  printk("physical_mm",
         "Size:  0x%x",
         ((uint32_t) &kernel_end) - ((uint32_t) &kernel_start));
}

ALWAYS_INLINE static void
physical_mm_initialize_region(uint32_t start, uint32_t length)
{
  /* Get the location of the start address in the bitmap */
  uint32_t bit = start / BLOCK_SIZE;
  uint32_t n_blocks = length / BLOCK_SIZE;

  for (; n_blocks > 0; n_blocks--)
    if (physical_mm_test_bit(bit, memory_map))
      physical_mm_set_usable(bit++, &total_free_blocks, memory_map);

  /* First block is always used (first 64KiB) */
  if (!physical_mm_test_bit(0, memory_map))
    physical_mm_set_used(0, &total_free_blocks, memory_map);
}

ALWAYS_INLINE static void
physical_mm_deinitialize_region(uint32_t start, uint32_t length)
{
  uint32_t bit = start / BLOCK_SIZE;
  uint32_t n_blocks = length / BLOCK_SIZE;

  if (length % BLOCK_SIZE > 0)
    n_blocks++;

  for (; n_blocks > 0; n_blocks--)
    physical_mm_set_used(bit++, &total_free_blocks, memory_map);
}

void
physical_mm_init(void)
{
  free_memory_regions_t *free_memory_regions = memory_map_get_free_regions();
  physical_mm_log_memory_map(free_memory_regions);

  spinlock_acquire(&memory_map_lock);

  /* All blocks are initially used */
  /* TODO: Move this block to a place after block_count is set. This is why
   * using block_count instead of MAX_BLOCKS wasn't working. */
  for (uint32_t i = 0; i < MAX_BLOCKS / BITMAP_ENTRY_SIZE; i++)
    memory_map[i] = 0xffffffff;

  uint32_t total_free_memory = 0;
  for (int i = 0; i < free_memory_regions->n_regions; i++) {
    multiboot_memory_map_t *region = free_memory_regions->region_list[i];
    total_free_memory += region->len_low;
    physical_mm_initialize_region(region->addr_low, region->len_low);
  }

  uint32_t kernel_size = ((uint32_t) &kernel_end) - ((uint32_t) &kernel_start);
  physical_mm_deinitialize_region((uint32_t) &kernel_start, kernel_size);

  /* Deinitialize first 4MiB */
  physical_mm_deinitialize_region(0, 4 * MiB);

  spinlock_release(&memory_map_lock);

  total_free_memory -= kernel_size;
  block_count = total_free_memory / BLOCK_SIZE;
  printk("\nphysical_mm", "Total blocks: 0x%x", block_count);
  printk("physical_mm", "Total free blocks: 0x%x", total_free_blocks);

#if 0
  /* Manually loop through and calculate the number of free blocks. */
  uint32_t free_blcks = 0;
  for (uint32_t i = 0; i < MAX_BLOCKS / BITMAP_ENTRY_SIZE; i++)
    /* At least one block in the entry isn't in use */
    if (memory_map[i] != 0xffffffff)
      /* Test each bit to see if it's zero */
      for (uint32_t j = 0; j < BITMAP_ENTRY_SIZE; j++)
        if (!physical_mm_test_bit(i * BITMAP_ENTRY_SIZE + j, memory_map))
          free_blcks++;

  printk("physical_mm",
         "Experimentally calculated free blocks: 0x%x",
         free_blcks);
#endif
}

uint32_t
physical_mm_find_free_block(void)
{
  /* TODO: Why doesn't using block_count instead of MAX_BLOCKS work? */
  for (uint32_t i = 0; i < MAX_BLOCKS / BITMAP_ENTRY_SIZE; i++)
    /* At least one block in the entry isn't in use */
    if (memory_map[i] != 0xffffffff)
      /* Test each bit to see if it's zero */
      for (uint32_t j = 0; j < BITMAP_ENTRY_SIZE; j++)
        if (!physical_mm_test_bit(i * BITMAP_ENTRY_SIZE + j, memory_map))
          return i * BITMAP_ENTRY_SIZE + j;

  /* Shouldn't be reached, since we're keeping track of the number of free
   * blocks */
  ASSERT_NOT_REACHED();
  return -1;
}

void *
physical_mm_allocate_block(void)
{
  if (total_free_blocks == 0) {
    printk("physical_mm", "No more free blocks!");
    return NULL;
  }

  spinlock_acquire(&memory_map_lock);

  uint32_t block = physical_mm_find_free_block();
  physical_mm_set_used(block, &total_free_blocks, memory_map);

  spinlock_release(&memory_map_lock);

  uint32_t physical_address = block * BLOCK_SIZE;
  return (void *) physical_address;
}

void
physical_mm_free_block(void *physical_address)
{
  uint32_t block = ((uint32_t) physical_address) / BLOCK_SIZE;
  physical_mm_set_usable(block, &total_free_blocks, memory_map);
}
