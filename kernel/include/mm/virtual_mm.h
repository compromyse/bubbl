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

#include <common.h>
#include <stdbool.h>
#include <stdint.h>

#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024
#define PAGE_SIZE (4 * KiB)

#define PDE_PRESENT(x) x
#define PDE_WRITABLE(x) ((x) << 1)
#define PDE_USER(x) ((x) << 2)
#define PDE_WRITETHROUGH(x) ((x) << 3)
#define PDE_CACHE_DISABLE(x) ((x) << 4)
#define PDE_ACCESSED(x) ((x) << 5)
/* Never set (reserved by Intel) */
#define PDE_RESERVED(x) ((x) << 6)
#define PDE_PAGE_SIZE(x) ((x) << 7)
/* NOTE: Unused by the CPU, free to be used by us! */
#define PDE_UNUSED(x) ((x) << 8)
/* Page table address */
#define PDE_FRAME(x) ((x) & 0x7ffff000)

#define PDE_IS_PRESENT(pd_entry) ((*pd_entry) & 1)
#define GET_PD_INDEX(virtual_address) (((uint32_t) virtual_address) >> 22)
#define PDE_GET_TABLE(pd_entry) ((*pd_entry) & ~0xfff)

#define PTE_PRESENT(x) x
#define PTE_WRITABLE(x) ((x) << 1)
#define PTE_USER(x) ((x) << 2)
#define PTE_WRITETHROUGH(x) ((x) << 3)
#define PTE_CACHE_DISABLE(x) ((x) << 4)
#define PTE_ACCESSED(x) ((x) << 5)
#define PTE_DIRTY(x) ((x) << 6)
#define PTE_PAT(x) ((x) << 7)
#define PTE_GLOBAL(x) ((x) << 8)
/* NOTE: Unused by the CPU, free to be used by us! */
#define PTE_UNUSED(x) ((x) << 9)
/* Left shift by 12 because we only need the bits from the twelfth bit. */
#define PTE_FRAME(x) ((x) & 0x7ffff000)

#define PTE_IS_PRESENT(pt_entry) ((*pt_entry) & 1)
#define GET_PTE_FRAME(x) ((x) >> 12)
#define GET_PT_INDEX(virtual_address)                                         \
  ((((uint32_t) virtual_address) >> 12) & 0x3ff)

#define ADD_ATTRIB(entry, attribute) (*entry |= (attribute))

#define VIRTUAL_ADDRESS(pd_index, pt_index)                                   \
  (((pd_index) << 22) | ((pt_index) << 12))

uint32_t *vmm_get_page_directory(void);

/*
 * Loads a given page directory into CR0
 */
void vmm_load_page_directory(uint32_t *page_directory);

/*
 * Switches the current page directory to a given page directory
 */
bool vmm_switch_page_directory(uint32_t *page_directory);

/*
 * Initialize the virtual memory manager
 */
void vmm_initialize(void);

/*
 * Map a physical address to a virtual address
 */
void vmm_map_page(void *physical_address, void *virtual_address);

/*
 * Unmap a page starting at virtual address
 */
void vmm_unmap_page(void *virtual_address);

/*
 * Find a virtual address with n consecutive free addresses.
 */
void *vmm_find_free_pages(uint32_t n_pages);

/*
 * Allocate and map n pages.
 */
void *vmm_alloc_pages(uint32_t n_pages);

/*
 * Free n pages from the starting address.
 */
void vmm_free_pages(void *starting_address, uint32_t n_pages);

#endif
