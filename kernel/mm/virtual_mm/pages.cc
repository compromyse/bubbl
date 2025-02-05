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

#include <kernel/halt.h>
#include <libk/stdio.h>
#include <mm/page_table_allocator.h>
#include <mm/physical_mm.h>
#include <mm/virtual_mm.h>
#include <stdbool.h>
#include <stdint.h>

namespace VirtualMM
{

void *
alloc_pages(uint32_t n_pages)
{
  uint32_t starting_address = (uint32_t) find_free_pages(n_pages);
  if (!starting_address)
    return NULL;

  for (uint32_t i = 0; i < n_pages; i++) {
    void *physical_address = PhysicalMM::allocate_block();
    void *virtual_address = (void *) (starting_address + (i * PAGE_SIZE));
    map_page(physical_address, virtual_address);
  }

  return (void *) starting_address;
}

void
free_pages(void *starting_address, uint32_t n_pages)
{
  for (uint32_t i = 0; i < n_pages; i++)
    unmap_page((void *) (((uint32_t) starting_address) + (i * 4096)));
}

}
