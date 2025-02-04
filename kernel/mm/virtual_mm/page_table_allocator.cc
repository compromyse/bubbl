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

#include <common.h>
#include <kernel/halt.h>
#include <libk/stdio.h>
#include <mm/page_table_allocator.h>
#include <mm/physical_mm.h>
#include <mm/virtual_mm.h>
#include <stddef.h>

namespace PageTableAllocator
{

uint32_t *l_page_directory = 0;
uint32_t *l_heap = NULL;
uint16_t l_table_index = 0;

static void
make_table(uint32_t *table_address)
{
  uint32_t *table = table_address;
  for (uint32_t i = 0; i < 1024; i++)
    table[i] = PTE_FRAME((uint32_t) PhysicalMM::allocate_block())
               | PTE_PRESENT(1) | PTE_WRITABLE(1);

  void *starting_address = VirtualMM::find_free_addresses(4 * KiB);
  uint32_t *pd_entry = &l_page_directory[GET_PD_INDEX(starting_address)];
  *pd_entry = PDE_FRAME((uint32_t) table) | PDE_PRESENT(1) | PDE_WRITABLE(1);

  l_heap = (uint32_t *) starting_address;
  l_table_index = 0;
}

void
prepare(void)
{
  if (l_page_directory != VirtualMM::get_page_directory())
    l_page_directory = VirtualMM::get_page_directory();

  /* Initial table */
  if (l_heap == NULL)
    make_table((uint32_t *) (7 * MiB));

  /* If we're gonna run out of space, allocate a new table with the remaining
   * page */
  if (l_table_index == 1023)
    make_table(allocate());
}

uint32_t *
allocate(void)
{
  uint32_t *next_table = l_heap + (l_table_index * 4 * KiB);
  l_table_index++;
  return next_table;
}

}
