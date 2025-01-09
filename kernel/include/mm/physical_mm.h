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

#include <stdbool.h>
#include <stdint.h>

#include <common.h>

/* TODO: Update this to 2MiB when PAE is enabled */
#define BLOCK_SIZE (4 * KiB)

#define BITMAP_ENTRY_SIZE 32

/* TODO: This is the maximum number of blocks for a 4GiB system. */
#define MAX_BLOCKS 1048576

void physical_mm_init(void);

uint32_t physical_mm_find_first_free_block(void);

void *physical_mm_allocate_block(void);
void physical_mm_free_block(void *physical_address);

void physical_mm_set_used(const uint32_t bit,
                          uint32_t *total_free_blocks,
                          uint32_t *memory_map);
void physical_mm_set_usable(const uint32_t bit,
                            uint32_t *total_free_blocks,
                            uint32_t *memory_map);
bool physical_mm_test_bit(const uint32_t bit, uint32_t *memory_map);

#endif
