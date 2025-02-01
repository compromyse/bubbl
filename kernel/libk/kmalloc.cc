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

#include <libk/kmalloc.h>
#include <libk/stdio.h>
#include <mm/virtual_mm.h>
#include <stddef.h>
#include <stdint.h>

/* TODO: Kmalloc must have space for a page table *at all times*. */

memory_chunk_t *starting_mc = NULL;
bool initialized = false;

memory_chunk_t *
add_block(void *address, uint32_t size)
{
  memory_chunk_t *mc = (memory_chunk_t *) address;
  mc->next = NULL;
  mc->prev = NULL;

  uint32_t chunk_size = size - sizeof(memory_chunk_t);
  printk("kmalloc", "size(0x%x) chunk_size(0x%x)");

  mc->size = chunk_size;
  printk("kmalloc",
         "mc(0x%x) mc->size(0x%x) chunk_size(0x%x)",
         mc,
         mc->size,
         chunk_size);
  return mc;
}

void
kmalloc_initialize(void)
{
  int *initial_region = (int *) VirtualMM::alloc_pages(1);
  printk("debug", "Initial region: 0x%x", initial_region);
  *initial_region = 10;

  /* memory_chunk_t *mc = (memory_chunk_t *) initial_region; */
  /* mc->size = 10; */
  /* printk("kmalloc", "mc->size(0x%x)", mc->size); */

  /* starting_mc = add_block(initial_region, 4 * PAGE_SIZE); */
  initialized = true;
}

bool
kmalloc_initialized(void)
{
  return initialized;
}

void *
kmalloc(uint32_t size)
{
  if (!initialized)
    kmalloc_initialize();

  /* printk("kmalloc", */
  /*        "Initialized region with starting_mc(0x%x) and size(0x%x)", */
  /*        starting_mc, */
  /*        starting_mc->size); */

  (void) size;
  return NULL;
}
