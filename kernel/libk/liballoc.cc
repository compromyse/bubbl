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

#include <libk/liballoc.h>
#include <libk/stdio.h>
#include <libk/string.h>
#include <mm/virtual_mm.h>
#include <stddef.h>

/* TODO: Kmalloc must have space for a page table *at all times*. */

namespace LibAlloc
{

/* Block */
void
Block::initialize(uint32_t size)
{
  memset(this, 0, sizeof(Block));

  m_size = size - sizeof(Block);

  m_next = NULL;
  m_prev = NULL;
}

inline void *
Block::get_chunk(void)
{
  return this + sizeof(Block);
}

/* LibAlloc */

bool l_initialized = false;
uint32_t l_heap_size = 0;

Block *l_first_block = 0;

bool
initialized(void)
{
  return l_initialized;
}

void
initialize(void)
{
  l_first_block = (Block *) VirtualMM::alloc_pages(1);
  l_first_block->initialize(PAGE_SIZE);

  l_heap_size += l_first_block->m_size;

  l_initialized = true;
}

}
