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

#include <stdint.h>

#include <libk/stdio.h>

#include <mm/physical_mm.h>
#include <mm/virtual_mm.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t initial_page_directory[PAGE_DIRECTORY_SIZE];
uint32_t initial_page_table[PAGE_TABLE_SIZE];

bool
virtual_mm_allocate_page(uint32_t *pt_entry)
{
  void *ptr = physical_mm_allocate_block();
  if (!ptr)
    return false;

  ADD_ATTRIB(pt_entry, SET_PTE_FRAME((uint32_t) ptr));
  ADD_ATTRIB(pt_entry, SET_PTE_PRESENT(1));

  return true;
}

bool
virtual_mm_free_page(uint32_t *pt_entry)
{
  void *ptr = (void *) GET_PTE_FRAME(pt_entry);
  if (ptr)
    physical_mm_free_block(ptr);

  *pt_entry = 0;

  return true;
}

ALWAYS_INLINE uint32_t *
virtual_mm_lookup_table(uint32_t *page_table, uint32_t virtual_addr)
{
  if (page_table)
    return &page_table[PAGE_TABLE_INDEX(virtual_addr)];

  return NULL;
}

ALWAYS_INLINE uint32_t *
virtual_mm_lookup_directory(uint32_t *page_directory, uint32_t virtual_addr)
{
  if (page_directory)
    return &page_directory[PAGE_DIRECTORY_INDEX(virtual_addr)];

  return NULL;
}
