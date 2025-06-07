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
#include <libk/liballoc.h>
#include <libk/stdio.h>
#include <mm/page_table_allocator.h>
#include <mm/physical_mm.h>
#include <mm/virtual_mm.h>
#include <stdbool.h>
#include <stdint.h>

namespace VirtualMM
{

extern uint32_t kernel_start;
extern uint32_t kernel_end;

uint32_t *l_current_page_directory = 0;

/* Kernel's page directory */
uint32_t l_page_directory[1024] ALIGNED(4096);
/* Page table for the first 4 MiB */
uint32_t l_fourMiB_page_table[1024] ALIGNED(4096);
/* Page table for the next 4 MiB */
uint32_t l_eightMiB_page_table[1024] ALIGNED(4096);

uint32_t *
get_page_directory(void)
{
  return l_current_page_directory;
}

ALWAYS_INLINE void
load_page_directory(uint32_t *page_directory)
{
  __asm__ volatile("movl %0, %%cr3" ::"r"(page_directory));
}

bool
switch_page_directory(uint32_t *page_directory)
{
  if (!page_directory)
    return false;
  l_current_page_directory = page_directory;
  load_page_directory(page_directory);

  return true;
}

ALWAYS_INLINE static void
enable_paging(void)
{
  __asm__ volatile("movl %%cr0, %%eax;"
                   "orl $0x80000000, %%eax;"
                   "movl %%eax, %%cr0" ::
                       : "eax");
}

void
initialize(void)
{
  /* Zero out the page tables and directories */
  for (uint32_t i = 0; i < 1024; i++) {
    l_fourMiB_page_table[i] = 0;
    l_eightMiB_page_table[i] = 0;
    l_page_directory[i] = 0;
  }

  /* Identity map the first 4MiB, excluding the 4th MiB
   * (maps 4KiB 1024 times) */
  for (uint32_t i = 0; i < 1024; i++)
    l_fourMiB_page_table[i]
        = PTE_FRAME(i << 12) | PTE_PRESENT(1) | PTE_WRITABLE(1);

  /* Identity map the next 4MiB */
  for (uint32_t i = 0; i < 1024; i++)
    l_eightMiB_page_table[i]
        = PTE_FRAME((i + 1024) << 12) | PTE_PRESENT(1) | PTE_WRITABLE(1);

  /* Set up the page directory entries */
  uint32_t *fourMiB_pd_entry = &l_page_directory[0];
  *fourMiB_pd_entry = PDE_FRAME((uint32_t) l_fourMiB_page_table)
                      | PDE_PRESENT(1) | PDE_WRITABLE(1);

  uint32_t *eightMiB_pd_entry = &l_page_directory[1];
  *eightMiB_pd_entry = PDE_FRAME((uint32_t) l_eightMiB_page_table)
                       | PDE_PRESENT(1) | PDE_WRITABLE(1);

  switch_page_directory(l_page_directory);
  enable_paging();

  PageTableAllocator::initialize();
}

uint32_t *
make_table(uint32_t *pd_entry)
{
  uint32_t *table = PageTableAllocator::allocate();

  for (uint32_t i = 0; i < 1024; i++)
    table[i] = 0x0;

  *pd_entry = PDE_FRAME((uint32_t) table) | PDE_PRESENT(1) | PDE_WRITABLE(1);
  return table;
}

ALWAYS_INLINE static uint32_t *
get_or_make_table(uint32_t *pd_entry)
{
  uint32_t *table = 0;

  if (!PDE_IS_PRESENT(pd_entry))
    table = make_table(pd_entry);
  else
    table = (uint32_t *) PDE_GET_TABLE(pd_entry);

  return table;
}

void
map_page(void *physical_address, void *virtual_address)
{
  uint32_t *pd_entry
      = &l_current_page_directory[GET_PD_INDEX(virtual_address)];
  uint32_t *table = get_or_make_table(pd_entry);

  uint32_t *pt_entry = &table[GET_PT_INDEX(virtual_address)];
  if (PTE_IS_PRESENT(pt_entry))
    /* Mapping previously mapped memory */
    ASSERT_NOT_REACHED();

  *pt_entry = PTE_FRAME((uint32_t) physical_address) | PTE_PRESENT(1)
              | PTE_WRITABLE(1);
}

void
unmap_page(void *virtual_address)
{
  uint32_t *pd_entry
      = &l_current_page_directory[GET_PD_INDEX(virtual_address)];

  uint32_t *table = 0;
  /* If the pd_entry isn't present, return */
  if (!PDE_IS_PRESENT(pd_entry))
    return;

  table = (uint32_t *) PDE_GET_TABLE(pd_entry);

  uint32_t *pt_entry = &table[GET_PT_INDEX(virtual_address)];
  printk("debug", "Freeing: 0x%x", pt_entry);
  *pt_entry = 0;
}

void *
find_free_pages(uint32_t n_pages)
{
  /* Skip the first two page directory entries; we don't wanna touch the first
   * 8MiB. */
  for (uint32_t pd_index = 2; pd_index < PAGE_DIRECTORY_SIZE; pd_index++) {
    uint32_t starting_pd_index = pd_index;
    uint32_t *pd_entry = &l_current_page_directory[pd_index];
    uint32_t *table = 0;

    bool table_is_present = PDE_IS_PRESENT(pd_entry);
    if (table_is_present)
      table = (uint32_t *) PDE_GET_TABLE(pd_entry);

    for (uint32_t starting_pt_index = 0; starting_pt_index < PAGE_TABLE_SIZE;
         starting_pt_index++) {
      uint32_t count = 0;
      if (table_is_present)
        if (PTE_IS_PRESENT(&table[starting_pt_index]))
          continue;

      /* We found our starting pt_entry */
      for (uint32_t pt_index = starting_pt_index; pt_index <= PAGE_TABLE_SIZE;
           pt_index++) {
        /* If we overflow, switch to the consecutive page directory entry */
        if (pt_index == PAGE_TABLE_SIZE) {
          pd_index++;
          if (pd_index == PAGE_DIRECTORY_SIZE)
            return 0; /* Ran out of pd_entries */

          pd_entry = &l_current_page_directory[pd_index];
          table_is_present = PDE_IS_PRESENT(pd_entry);
          pt_index = 0;
        }

        /* If the table is present, and if the PTE is present, then break */
        if (table_is_present)
          if (PTE_IS_PRESENT(&table[pt_index])) {
            /* Since we have some used address at some point between j and
             * count, we can't find n consecutive free addresses in between j
             * and the used block (j + count + 1) */
            starting_pt_index += count;
            break;
          }

        /* TODO: This can be easily optimized if the table is not present.
         * (count += 4096, since we know that the table is not present) */
        count++;

        if (count == n_pages)
          return (void *) VIRTUAL_ADDRESS(starting_pd_index,
                                          starting_pt_index);
      }
    }
  }

  ASSERT_NOT_REACHED();
  return 0;
}

}
