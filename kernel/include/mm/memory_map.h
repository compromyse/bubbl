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

#ifndef __mm_memory_map_h
#define __mm_memory_map_h

#include <mm/multiboot.h>
#include <stdint.h>

/* TODO: Practically, do we need more than 32? */
#define MAX_FREE_REGIONS 32

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint8_t n_regions;
  multiboot_memory_map_t *region_list[MAX_FREE_REGIONS];
} free_memory_regions_t;

void memory_map_load(multiboot_info_t *mmap);
free_memory_regions_t *memory_map_get_free_regions(void);

#ifdef __cplusplus
}
#endif

#endif
