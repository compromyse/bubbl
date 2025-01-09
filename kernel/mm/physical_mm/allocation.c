#include <stdbool.h>
#include <stdint.h>

#include <libk/stdio.h>

#include <mm/physical_mm.h>

#include <kernel/halt.h>

uint32_t
physical_mm_find_first_free_block(uint32_t *memory_map)
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
physical_mm_allocate_block(uint32_t *total_free_blocks, uint32_t *memory_map)
{
  if (total_free_blocks == 0) {
    printk("physical_mm", "No more free blocks!");
    return NULL;
  }

  uint32_t block = physical_mm_find_first_free_block(memory_map);

  physical_mm_set_used(block, total_free_blocks, memory_map);

  uint32_t physical_address = block * BLOCK_SIZE;
  return (void *) physical_address;
}

void
physical_mm_free_block(void *physical_address,
                       uint32_t *total_free_blocks,
                       uint32_t *memory_map)
{
  uint32_t block = ((uint32_t) physical_address) / BLOCK_SIZE;
  physical_mm_set_usable(block, total_free_blocks, memory_map);
}
