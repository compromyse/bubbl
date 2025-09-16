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

#ifndef __mm_physical_mm_h
#define __mm_physical_mm_h

#include <common.h>
#include <stdbool.h>
#include <stdint.h>

/* TODO: Update this to 2MiB when PAE is enabled */
#define BLOCK_SIZE (4 * KiB)

#define BITMAP_ENTRY_SIZE 32

/* This is the maximum number of blocks for a 4GiB system. */
#define MAX_BLOCKS 1048576

namespace PhysicalMM
{

/* Initialize the memory map by getting all free regions, setting all blocks to
 * used, initializing regions marked free by the memory map provided by
 * multiboot, and deinitializing the memory used by the kernel */
void initialize(void);

/* Find and allocate a free memory block, returning the physical address of the
 * block */
void *allocate_block(void);

/* Free an allocated memory block, given the physical address (from
 * allocate_block) */
void free_block(void *physical_address);

/*-- BITMAP --*/

/* Marks the block as 'used' */
void set_used(const uint32_t bit,
              uint32_t *total_free_blocks,
              uint32_t *memory_map);

/* Marks the block as 'unused' */
void set_usable(const uint32_t bit,
                uint32_t *total_free_blocks,
                uint32_t *memory_map);

/* Returns:
 * True if the bit is set (block is in use)
 * False if the bit is unset (block isn't in use)
 */
bool test_bit(const uint32_t bit, uint32_t *memory_map);

}

#endif
