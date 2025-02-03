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
#include <mm/memory_map.h>
#include <mm/multiboot.h>
#include <stdint.h>

namespace MemoryMap
{

static free_memory_regions_t l_free_memory_regions = { 0 };

ALWAYS_INLINE static char *
fetch_type(multiboot_memory_map_t *mmap)
{
  switch (mmap->type) {
  case MULTIBOOT_MEMORY_AVAILABLE:
    return "AVAILABLE";
  case MULTIBOOT_MEMORY_RESERVED:
    return "RESERVED";
  case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
    return "ACPI_RECLAIMABLE";
  case MULTIBOOT_MEMORY_NVS:
    return "NVS";
  case MULTIBOOT_MEMORY_BADRAM:
    return "BADRAM";
  default:
    return "UNKNOWN";
  }
}

void
load(multiboot_info_t *multiboot_info)
{
  printk("mm", "Loading Memory Map:");

  uint32_t total_mem = 0;
  uint32_t total_available_mem = 0;

  /* https://www.gnu.org/software/grub/manual/multiboot/multiboot.html:
   *
   * If bit 6 in the ‘flags’ word is set, then the ‘mmap_*’ fields are
   * valid, and indicate the address and length of a buffer containing a
   * memory map of the machine provided by the BIOS. ‘mmap_addr’ is the
   * address, and ‘mmap_length’ is the total size of the buffer. The buffer
   * consists of one or more of the following size/structure pairs (‘size’
   * is really used for skipping to the next pair): */

  if (!(multiboot_info->flags & MULTIBOOT_INFO_ELF_SHDR)) {
    printk("mm", "Invalid memory map!");
    halt();
  }

  for (uint32_t i = 0; i < multiboot_info->mmap_length;
       i += sizeof(multiboot_memory_map_t)) {
    multiboot_memory_map_t *mmap
        = (multiboot_memory_map_t *) (multiboot_info->mmap_addr + i);

    total_mem += mmap->len_low;
    if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
      l_free_memory_regions.region_list[l_free_memory_regions.n_regions]
          = mmap;
      l_free_memory_regions.n_regions++;

      total_available_mem += mmap->len_low;
    }

    printk("mm",
           "start: 0x%.08x | length: 0x%.08x | type: %s",
           mmap->addr_low,
           mmap->len_low,
           fetch_type(mmap));
  }

  printk("mm", "Total Memory: %lu MiB", total_mem / MiB);
  printk("mm", "Total Available Memory: %lu MiB", total_available_mem / MiB);
}

free_memory_regions_t *
get_free_regions(void)
{
  return &l_free_memory_regions;
}

}
