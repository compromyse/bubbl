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

#ifndef __mm_virtual_mm_h
#define __mm_virtual_mm_h

#include <stdbool.h>
#include <stdint.h>

#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024

#define PAGE_DIRECTORY_INDEX(virtual_address) ((virtual_address >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(virtual_address) ((virtual_address >> 12) & 0x3ff)

#define SET_PDE_PRESENT(x) x
#define SET_PDE_WRITABLE(x) ((x) << 1)
#define SET_PDE_USER(x) ((x) << 2)
#define SET_PDE_WRITETHROUGH(x) ((x) << 3)
#define SET_PDE_CACHE_DISABLE(x) ((x) << 4)
#define SET_PDE_ACCESSED(x) ((x) << 5)
/* Never set (reserved by Intel) */
#define SET_PDE_RESERVED(x) ((x) << 6)
#define SET_PDE_PAGE_SIZE(x) ((x) << 7)
/* NOTE: Unused by the CPU, free to be used by us! */
#define SET_PDE_UNUSED(x) ((x) << 8)
/* Page table address */
#define SET_PDE_FRAME(x) ((x) << 12)

#define GET_PDE_FRAME(x) ((*x) >> 12)

#define SET_PTE_PRESENT(x) x
#define SET_PTE_WRITABLE(x) ((x) << 1)
#define SET_PTE_USER(x) ((x) << 2)
#define SET_PTE_WRITETHROUGH(x) ((x) << 3)
#define SET_PTE_CACHE_DISABLE(x) ((x) << 4)
#define SET_PTE_ACCESSED(x) ((x) << 5)
#define SET_PTE_DIRTY(x) ((x) << 6)
#define SET_PTE_PAT(x) ((x) << 7)
#define SET_PTE_GLOBAL(x) ((x) << 8)
/* NOTE: Unused by the CPU, free to be used by us! */
#define SET_PTE_UNUSED(x) ((x) << 9)
/* MAX: 0xFFFFF000 */
#define SET_PTE_FRAME(x) ((x) << 12)

#define PTE_IS_PRESENT(x) ((x) &1)
#define GET_PTE_FRAME(x) ((*x) >> 12)

#define ADD_ATTRIB(entry, attribute) (*entry |= (attribute))

/*
 * Loads a given page directory into CR0
 */
void virtual_mm_load_page_directory(uint32_t *page_directory);

/*
 * Switches the current page directory to a given page directory
 */
bool virtual_mm_switch_page_directory(uint32_t *page_directory);

/*
 * Initialize the virtual memory manager
 */
void virtual_mm_initialize(void);

#endif
