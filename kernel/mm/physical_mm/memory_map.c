#include <stdbool.h>
#include <stdint.h>

#include <mm/memory_map.h>
#include <mm/physical_mm.h>

/* Marks the block as 'used' */
ALWAYS_INLINE void
physical_mm_set_used(const uint32_t bit,
                     uint32_t *total_free_blocks,
                     uint32_t *memory_map)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  (*total_free_blocks)--;
  memory_map[memory_map_index] |= bitmask;
}

/* Marks the block as 'unused' */
ALWAYS_INLINE void
physical_mm_set_usable(const uint32_t bit,
                       uint32_t *total_free_blocks,
                       uint32_t *memory_map)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  (*total_free_blocks)++;
  memory_map[memory_map_index] &= ~bitmask;
}

/* Returns:
 * True if the bit is set (block is in use)
 * False if the bit is unset (block isn't in use)
 */
ALWAYS_INLINE bool
physical_mm_test_bit(const uint32_t bit, uint32_t *memory_map)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  return memory_map[memory_map_index] & bitmask;
}
