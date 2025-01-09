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
uint32_t memory_map[MAX_BLOCKS / BITMAP_ENTRY_SIZE];

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

ALWAYS_INLINE static void
physical_mm_initialize_region(uint32_t start, uint32_t length)
{
  /* Get the location of the start address in the bitmap */
  uint32_t bit = start / BLOCK_SIZE;
  uint32_t n_blocks = length / BLOCK_SIZE;

  for (; n_blocks > 0; n_blocks--)
    if (physical_mm_test_bit(bit, memory_map))
      physical_mm_set_usable(bit++, &total_free_blocks, memory_map);

  /* First block is always used (first 64 KiB) */
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
    if (!physical_mm_test_bit(bit, memory_map))
      physical_mm_set_used(bit++, &total_free_blocks, memory_map);
}

void
physical_mm_init(void)
{
  free_memory_regions_t *free_memory_regions = memory_map_get_free_regions();
  physical_mm_log_memory_map(free_memory_regions);

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
