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

ALWAYS_INLINE void
virtual_mm_load_page_directory(uint32_t *page_directory)
{
  __asm__("movl %0, %%eax" ::"r"(page_directory) : "%eax");
  /* For some reason, that colon is required; else the assembler thinks that
   * %eax doesn't exist :^) */
  __asm__("movl %%eax, %%cr3" ::: "%eax");
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

/* TODO: I have absolutely _no_ idea if this works */
void
virtual_mm_flush_tlb_entry(uint32_t *virtual_addr)
{
  __asm__("cli;"
          "invlpg (%0);"
          "sti" ::"r"(virtual_addr));
}

bool
virtual_mm_map_page(uint32_t *physical_addr, uint32_t *virtual_addr)
{
  uint32_t *pd_entry
      = &current_page_directory[PAGE_DIRECTORY_INDEX((uint32_t) virtual_addr)];

  /* If the page directory entry isn't present */
  if (!PTE_IS_PRESENT(*pd_entry)) {
    uint32_t *page_table = physical_mm_allocate_block();
    if (!page_table)
      return false;

    *pd_entry = 0;
    ADD_ATTRIB(pd_entry, SET_PDE_PRESENT(1));
    ADD_ATTRIB(pd_entry, SET_PDE_WRITABLE(1));
    ADD_ATTRIB(pd_entry, SET_PDE_FRAME((uint32_t) page_table));
  }

  uint32_t *page_table = (uint32_t *) GET_PDE_FRAME(pd_entry);
  uint32_t *pt_entry = &page_table[PAGE_TABLE_INDEX((uint32_t) virtual_addr)];
  *pt_entry = 0;

  ADD_ATTRIB(pt_entry, SET_PTE_PRESENT(1));
  ADD_ATTRIB(pt_entry, SET_PTE_FRAME((uint32_t) physical_addr));

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

  /* Identity map the first 4MiB (maps 4KiB 1024 times) */
  for (uint32_t i = 0, physical_addr = 0, virtual_addr = 0; i < 1024;
       i++, physical_addr += 4096, virtual_addr += 4096) {
    uint32_t page = 0;
    ADD_ATTRIB(&page, SET_PTE_PRESENT(1));
    ADD_ATTRIB(&page, SET_PTE_FRAME(physical_addr));

    table[PAGE_TABLE_INDEX(virtual_addr)] = page;
  }

  uint32_t *page_directory = physical_mm_allocate_block();
  if (!page_directory)
    ASSERT_NOT_REACHED();

  for (uint32_t i = 0; i < 1024; i++)
    page_directory[i] = 0;

  uint32_t *pd_entry = &page_directory[PAGE_DIRECTORY_INDEX(0)];
  ADD_ATTRIB(pd_entry, SET_PDE_PRESENT(1));
  ADD_ATTRIB(pd_entry, SET_PDE_WRITABLE(1));
  ADD_ATTRIB(pd_entry, SET_PDE_FRAME((uint32_t) table));

  current_page_directory = page_directory;
  virtual_mm_switch_page_directory(current_page_directory);
  virtual_mm_enable_paging();
}
