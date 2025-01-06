/* Bitmap Based Allocation:
 * https://web.archive.org/web/20190316115948/http://www.brokenthorn.com/Resources/OSDev17.html
 */

#include <stdbool.h>
#include <stdint.h>

#include <libk/stdio.h>

#include <mm/memory_map.h>
#include <mm/physical_mm.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t block_count;
uint32_t *memory_map;

void
physical_mm_init(void)
{
  free_memory_regions_t *free_memory_regions = memory_map_get_free_regions();
  printk("physical_mm", "Free Memory Regions:");
  for (int i = 0; i < free_memory_regions->n_regions; i++)
    printk("physical_mm",
           "start: 0x%.08x | length: 0x%.08x",
           free_memory_regions->region_list[i]->addr_low,
           free_memory_regions->region_list[i]->len_low);

  printk("physical_mm", "Kernel region:");
  printk("physical_mm", "Start: 0x%x", &kernel_start);
  printk("physical_mm", "End:   0x%x", &kernel_end);
  printk("physical_mm",
         "Size: 0x%x",
         ((uint32_t) &kernel_end) - ((uint32_t) &kernel_start));
}

ALWAYS_INLINE void
physical_mm_set(const uint32_t bit)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  memory_map[memory_map_index] |= bitmask;
}

ALWAYS_INLINE void
physical_mm_unset(const uint32_t bit)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  memory_map[memory_map_index] &= ~bitmask;
}

/* Returns:
 * True if the bit is unset (block isn't in use)
 * False if the bit is used (block is in use)
 */
ALWAYS_INLINE bool
physical_mm_test_bit(const uint32_t bit)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  return memory_map[memory_map_index] & bitmask;
}

uint32_t
physical_mm_find_first_free(void)
{
  for (uint32_t i = 0; i < block_count / BITMAP_ENTRY_SIZE; i++)
    /* All blocks in the entry aren't in use */
    if (memory_map[i] != 0xffffffff)
      /* Test each bit to see if it's zero */
      for (uint32_t j = 0; j < BITMAP_ENTRY_SIZE; j++)
        if (physical_mm_test_bit(i * BITMAP_ENTRY_SIZE + j))
          return i * BITMAP_ENTRY_SIZE + j;

  /* TODO: Assert not reached, or something similar */
  return -1;
}
