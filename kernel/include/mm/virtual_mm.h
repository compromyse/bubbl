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

#define PAGE_SIZE (4096 * KiB)
#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024

#define PAGE_DIRECTORY_INDEX(virtual_address) ((virtual_address >> 22) & 0x3ff)
#define PAGE_TABLE_INDEX(virtual_address) ((virtual_address >> 12) & 0x3ff)

#define PDE_PRESENT 1
#define PDE_WRITABLE (1 << 1)
#define PDE_USER (1 << 2)
#define PDE_WRITETHROUGH (1 << 3)
#define PDE_CACHE_DISABLE (1 << 4)
#define PDE_ACCESSED (1 << 5)
/* Never set (reserved by Intel) */
#define PDE_RESERVED (0 << 6)
#define PDE_PAGE_SIZE (1 << 7)
#define PDE_GLOBAL (1 << 8)
/* NOTE: Unused by the CPU, free to be used by us! */
#define PDE_UNUSED(x) (x << 9)
/* Page table address */
#define PDE_FRAME(x) (x << 11)

#define SET_PTE_PRESENT(x) x
#define SET_PTE_WRITABLE(x) (x << 1)
#define SET_PTE_USER(x) (x << 2)
#define SET_PTE_WRITETHROUGH(x) (x << 3)
#define SET_PTE_CACHE_DISABLE(x) (x << 4)
#define SET_PTE_ACCESSED(x) (x << 5)
#define SET_PTE_DIRTY(x) (x << 6)
#define SET_PTE_PAT(x) (x << 7)
#define SET_PTE_GLOBAL(x) (x << 8)
/* NOTE: Unused by the CPU, free to be used by us! */
#define SET_PTE_UNUSED(x) (x << 9)
/* MAX: 0xFFFFF000 */
#define SET_PTE_FRAME(x) (x << 11)

#define GET_PTE_FRAME(x) (*x >> 11)

#define ADD_ATTRIB(entry, attribute) (*entry |= (attribute))

/*
 * Given a page table entry, it sets the frame to a newly allocated address,
 * and sets the present bit to false.
 */
bool virtual_mm_allocate_page(uint32_t *pt_entry);

/*
 * Given a page table entry, it sets the entry to 0 (therefore zero-ing the
 * frame and present bit).
 */
bool virtual_mm_free_page(uint32_t *pt_entry);

/*
 * Given a page table and virtual address, it returns a pointer to the page
 * table entry referenced by the virtual address.
 */
uint32_t *virtual_mm_lookup_table(uint32_t *page_table, uint32_t virtual_addr);

/*
 * Given a page directory and virtual address, it returns a pointer to the page
 * directory entry referenced by the virtual address.
 */
uint32_t *virtual_mm_lookup_directory(uint32_t *page_directory,
                                      uint32_t virtual_addr);

/*
 * Loads a given page directory into CR0
 */
void virtual_mm_load_page_directory(uint32_t *page_directory);

#endif
