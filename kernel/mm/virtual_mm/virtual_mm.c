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

#include <kernel/halt.h>

#include <libk/stdio.h>

#include <mm/physical_mm.h>
#include <mm/virtual_mm.h>

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t *current_page_directory = 0;

ALWAYS_INLINE void
virtual_mm_load_page_directory(uint32_t *page_directory)
{
  __asm__("movl %0, %%cr3" ::"r"(page_directory));
}

bool
virtual_mm_switch_page_directory(uint32_t *page_directory)
{
  if (!page_directory)
    return false;
  current_page_directory = page_directory;
  virtual_mm_load_page_directory(page_directory);

  return true;
}

ALWAYS_INLINE static void
virtual_mm_enable_paging(void)
{
  __asm__("movl %%cr0, %%eax;"
          "orl $0x80000000, %%eax;"
          "movl %%eax, %%cr0" ::
              : "eax");
}

void
virtual_mm_initialize(void)
{
  uint32_t *table = physical_mm_allocate_block();
  if (!table)
    ASSERT_NOT_REACHED();

  for (uint32_t i = 0; i < 1024; i++)
    table[i] = 0;

  /* Identity map the first 4MiB, excluding the 4th MiB
   * (maps 4KiB 1024 times) */
  for (uint32_t i = 0; i < 1024; i++)
    table[i] = PTE_FRAME(i) | PTE_PRESENT(1) | PTE_WRITABLE(1);

  uint32_t *page_directory = physical_mm_allocate_block();
  if (!page_directory)
    ASSERT_NOT_REACHED();
  printk("debug", "Page directory is at: 0x%x", page_directory);

  for (uint32_t i = 0; i < 1024; i++)
    page_directory[i] = 0;

  uint32_t *pd_entry = &page_directory[0];
  *pd_entry = PDE_FRAME((uint32_t) table) | PDE_PRESENT(1) | PDE_WRITABLE(1);

  virtual_mm_switch_page_directory(page_directory);
  virtual_mm_enable_paging();
}

void
virtual_mm_map_page(void *physical_address, void *virtual_address)
{
  uint32_t *pd_entry = &current_page_directory[GET_PDE_FRAME(virtual_address)];

  uint32_t *table = 0;
  /* If the pd_entry isn't present, allocate a block for it, zero the table,
   * and set the pd_entry's frame to the table's address. */
  if (!PDE_IS_PRESENT(pd_entry)) {
    table = physical_mm_allocate_block();
    if (!table)
      ASSERT_NOT_REACHED();

    for (uint32_t i = 0; i < 1024; i++)
      table[i] = 0;

    *pd_entry = PDE_FRAME((uint32_t) table) | PDE_PRESENT(1) | PDE_WRITABLE(1);
  } else
    table = (uint32_t *) PDE_GET_TABLE(pd_entry);

  uint32_t *pt_entry = &table[GET_PTE_FRAME(virtual_address)];
  if (PTE_IS_PRESENT(pt_entry))
    ASSERT_NOT_REACHED(); /* TODO: Mapping previously mapped memory */

  *pt_entry = PTE_FRAME((uint32_t) physical_address) | PTE_PRESENT(1)
              | PTE_WRITABLE(1);
}
