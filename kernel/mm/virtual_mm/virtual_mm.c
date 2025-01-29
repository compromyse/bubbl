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

ALWAYS_INLINE uint32_t *
get_or_make_table(uint32_t *pd_entry)
{
  uint32_t *table;
  if (!PDE_IS_PRESENT(pd_entry)) {
    table = physical_mm_allocate_block();
    if (!table)
      ASSERT_NOT_REACHED();

    for (uint32_t i = 0; i < 1024; i++)
      table[i] = 0x0;

    *pd_entry = PDE_FRAME((uint32_t) table) | PDE_PRESENT(1) | PDE_WRITABLE(1);
  } else
    table = (uint32_t *) PDE_GET_TABLE(pd_entry);

  return table;
}

void
virtual_mm_map_page(void *physical_address, void *virtual_address)
{
  uint32_t *pd_entry = &current_page_directory[GET_PD_INDEX(virtual_address)];
  uint32_t *table = get_or_make_table(pd_entry);

  uint32_t *pt_entry = &table[GET_PT_INDEX(virtual_address)];
  if (PTE_IS_PRESENT(pt_entry)) {
    printk("debug", "Mapping previously mapped memory: 0x%x", pt_entry);
    ASSERT_NOT_REACHED();
  }

  *pt_entry = PTE_FRAME((uint32_t) physical_address) | PTE_PRESENT(1)
              | PTE_WRITABLE(1);
}

void
virtual_mm_unmap_page(void *virtual_address)
{
  uint32_t *pd_entry = &current_page_directory[GET_PD_INDEX(virtual_address)];

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
virtual_mm_find_free_virtual_addresses(uint32_t n)
{
  /* Skip the first page directory, we don't wanna touch the first 4MiB. */
  for (uint32_t pd_index = 1; pd_index < PAGE_DIRECTORY_SIZE; pd_index++) {
    uint32_t starting_pd_index = pd_index;
    uint32_t *pd_entry = &current_page_directory[pd_index];
    /* Ideally, we shouldn't be allocating tables here */
    uint32_t *table = get_or_make_table(pd_entry);

    for (uint32_t starting_pt_index = 0; starting_pt_index < PAGE_TABLE_SIZE;
         starting_pt_index++) {
      uint32_t *pt_entry = &table[starting_pt_index];
      if (PTE_IS_PRESENT(pt_entry))
        continue;

      /* We found our starting pt_entry */
      uint32_t count = 0;
      for (uint32_t pt_index = starting_pt_index; pt_index <= PAGE_TABLE_SIZE;
           pt_index++) {
        /* If we overflow, switch to the consecutive page directory entry */
        if (pt_index == PAGE_TABLE_SIZE) {
          pd_index++;
          if (pd_index == PAGE_DIRECTORY_SIZE)
            return 0; /* Ran out of pd_entries */

          pd_entry = &current_page_directory[pd_index];
          table = get_or_make_table(pd_entry);
          pt_index = 0;
        }

        /* If page table entry is already used, break from the current loop
         */
        uint32_t *pt_entry = &table[pt_index];
        if (PTE_IS_PRESENT(pt_entry)) {
          /* Since we have some used address at some point between j and
           * count, we can't find n consecutive free addresses in between j
           * and the used block (j + count + 1) */
          starting_pt_index += count;
          break;
        }

        if (++count == n)
          return (void *) VIRTUAL_ADDRESS(starting_pd_index,
                                          starting_pt_index);
      }
    }
  }

  ASSERT_NOT_REACHED();
  return 0;
}

void *
virtual_mm_alloc_pages(uint32_t n_pages)
{
  uint32_t starting_address
      = (uint32_t) virtual_mm_find_free_virtual_addresses(n_pages);
  if (starting_address == 0)
    return 0;

  for (uint32_t i = 0; i < n_pages; i++) {
    void *virtual_address = (void *) (starting_address + (i * PAGE_SIZE));
    void *physical_address = physical_mm_allocate_block();
    virtual_mm_map_page(physical_address, virtual_address);
  }

  return (void *) starting_address;
}

void
virtual_mm_free_pages(void *starting_address, uint32_t n_pages)
{
  for (uint32_t i = 0; i < n_pages; i++)
    virtual_mm_unmap_page(
        (void *) (((uint32_t) starting_address) + (i * 4096)));
}
