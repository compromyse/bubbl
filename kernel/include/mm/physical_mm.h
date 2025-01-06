/*
 * CMOS
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

#include <stdint.h>

#include <common.h>

/*
 * A byte has 8 bits, hence we can store information for up to 8 blocks at a
 * time
 */
#define BLOCKS_PER_BYTE 8

/* TODO: Update this to 2MiB when PAE is enabled */
#define BLOCK_SIZE 4 * KiB

#define BLOCK_ALIGN BLOCK_SIZE

#define BITMAP_ENTRY_SIZE 32

void physical_mm_init(void);
uint32_t mmap_find_first_free(void);

#endif
