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

#include <mm/physical_mm.h>
#include <stdbool.h>
#include <stdint.h>

namespace PhysicalMM
{

/* Marks the block as 'used' */
void
set_used(const uint32_t bit, uint32_t *total_free_blocks, uint32_t *memory_map)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  (*total_free_blocks)--;
  memory_map[memory_map_index] |= bitmask;
}

/* Marks the block as 'unused' */
void
set_usable(const uint32_t bit,
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
bool
test_bit(const uint32_t bit, uint32_t *memory_map)
{
  uint32_t memory_map_index = bit / BITMAP_ENTRY_SIZE;
  uint32_t bitmask = 1 << (bit % BITMAP_ENTRY_SIZE);
  return memory_map[memory_map_index] & bitmask;
}

}
