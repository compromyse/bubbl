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
#include <libk/kmalloc.h>
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
  PhysicalMM::init();
  VirtualMM::init();

  // void *x = virtual_mm_find_free_addresses(1046999);
  // printk("debug", "x(0x%x)", x);

#if 0
  int *x = PhysicalMM::allocate_block();
  /* *x = 20; */
  printk("debug", "x(0x%x)", x);

  /* virtual_mm_alloc_pages(1); */
  /* void *x = kmalloc(12); */
  /* printk("debug", "x(0x%x)", x); */

  printk("\nKernel", "Started.");
#endif

  exit();
  halt(); /* If exit() fails (on real hardware) */
}
