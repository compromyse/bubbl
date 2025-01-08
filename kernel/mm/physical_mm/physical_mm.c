/* Bitmap Based Allocation:
 * https://web.archive.org/web/20190316115948/http://www.brokenthorn.com/Resources/OSDev17.html
 */

/* TODO: Stack based allocation? */

#include <stdbool.h>
#include <stdint.h>

#include <libk/stdio.h>

#include <mm/memory_map.h>
#include <mm/physical_mm.h>

#include <kernel/halt.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t block_count = 0;
uint32_t total_free_blocks = 0;
uint32_t memory_map[MAX_BLOCKS];

ALWAYS_INLINE static void
physical_mm_log_memory_map(free_memory_regions_t *free_memory_regions)
{
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

/* Marks the block as 'used' */
ALWAYS_INLINE static void
physical_mm_set_unusable(const uint32_t bit)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  memory_map[memory_map_index] |= bitmask;
}

/* Marks the block as 'unused' */
ALWAYS_INLINE static void
physical_mm_set_usable(const uint32_t bit)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  memory_map[memory_map_index] &= ~bitmask;
}

ALWAYS_INLINE static void
physical_mm_initialize_region(uint32_t start, uint32_t length)
{
  /* Get the location of the start address in the bitmap */
  uint32_t bit = start / BLOCK_SIZE;
  uint32_t n_blocks = length / BLOCK_SIZE;

  for (; n_blocks > 0; n_blocks--) {
    physical_mm_set_usable(bit++);
    total_free_blocks++;
  }

  /* First block is always unusable (first 64 KiB) */
  physical_mm_set_unusable(0);
}

ALWAYS_INLINE static void
physical_mm_deinitialize_region(uint32_t start, uint32_t length)
{
  uint32_t align = start / BLOCK_SIZE;
  uint32_t n_blocks = length / BLOCK_SIZE;

  if (length % BLOCK_SIZE > 0)
    n_blocks++;

  for (; n_blocks > 0; n_blocks--) {
    physical_mm_set_unusable(align++);
    total_free_blocks--;
  }
}

void
physical_mm_init(void)
{
  free_memory_regions_t *free_memory_regions = memory_map_get_free_regions();
  physical_mm_log_memory_map(free_memory_regions);

  /* All blocks are initially unusable */
  for (uint32_t i = 0; i < MAX_BLOCKS; i++)
    memory_map[i] = 0xffffffff;

  uint32_t total_free_memory = 0;
  for (int i = 0; i < free_memory_regions->n_regions; i++) {
    multiboot_memory_map_t *region = free_memory_regions->region_list[i];
    total_free_memory += region->len_low;
    physical_mm_initialize_region(region->addr_low, region->len_low);
  }

  physical_mm_deinitialize_region((uint32_t) &kernel_start,
                                  ((uint32_t) &kernel_end)
                                      - ((uint32_t) &kernel_start));

  block_count = total_free_memory / BLOCK_SIZE;
  printk("\nphysical_mm", "Total blocks: 0x%x", block_count);
  printk("physical_mm", "Total free blocks: 0x%x", total_free_blocks);
}

/* Returns:
 * True if the bit is unset (block isn't in use)
 * False if the bit is set (block is in use)
 */
ALWAYS_INLINE static bool
physical_mm_test_bit(const uint32_t bit)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  return memory_map[memory_map_index] & bitmask;
}

uint32_t
physical_mm_find_first_free_block(void)
{
  for (uint32_t i = 0; i < block_count / BITMAP_ENTRY_SIZE; i++)
    /* At least one block in the entry isn't in use */
    if (memory_map[i] != 0xffffffff)
      /* Test each bit to see if it's zero */
      for (uint32_t j = 0; j < BITMAP_ENTRY_SIZE; j++)
        if (physical_mm_test_bit(i * BITMAP_ENTRY_SIZE + j))
          return i * BITMAP_ENTRY_SIZE + j;

  return -1;
}
