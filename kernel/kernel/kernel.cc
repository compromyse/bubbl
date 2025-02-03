/*
 * bubbl
 * Copyright (C) 2024-2025  Raghuram Subramani <raghus2247@gmail.com>
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

#include <boot/gdt.h>
#include <drivers/serial.h>
#include <drivers/vga_text_buffer.h>
#include <kernel/halt.h>
#include <libk/liballoc.h>
#include <libk/stdio.h>
#include <mm/memory_map.h>
#include <mm/multiboot.h>
#include <mm/physical_mm.h>
#include <mm/virtual_mm.h>
#include <stdint.h>

extern "C" void
kernel_main(uint32_t magic, multiboot_info_t *multiboot_info)
{
  Serial::initialize();
  VGATextBuffer::initialize();

  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    printk("Kernel", "Invalid Multiboot Magic: %x", magic);
    halt();
  }

  GDT::load();
  MemoryMap::load(multiboot_info);
  PhysicalMM::initialize();
  VirtualMM::initialize();
  LibAlloc::initialize();

  uint32_t *page = (uint32_t *) VirtualMM::alloc_pages(1);
  printk("debug", "page(0x%x)", page);
  page = (uint32_t *) VirtualMM::alloc_pages(1);
  printk("debug", "page(0x%x)", page);

  for (uint32_t i = 0; i < 1024; i++)
    page[i] = i;

  // for (uint32_t i = 0; i < 1024; i++)
  // if (page[i] != i)
  // printk("debug", "page[i](%lu) i(%lu)", page[i], i);

  // int *x = (int *) LibAlloc::kmalloc(sizeof(int) * 8192);
  // for (uint32_t i = 0; i < 8192; i++)
  //   x[i] = i;
  // printk("debug", "x(0x%x) *x(0x%x)", x, x[12]);

  printk("\nKernel", "Started.");

  exit();
  halt(); /* If exit() fails (on real hardware) */
}
