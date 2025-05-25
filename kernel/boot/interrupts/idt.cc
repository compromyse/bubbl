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

#include <boot/interrupts.h>
#include <kernel/io.h>
#include <libk/stdio.h>

namespace Interrupts
{

extern "C" void *isr_stub_table[];

entry_t l_entries[256];
descriptor_t descriptor = { sizeof(l_entries) - 1, l_entries };

void
load_idt(void)
{
  for (uint16_t i = 0; i < 256; i++)
    l_entries[i] = (entry_t) { 0 };

  /* The first 32 entries are exceptions */
  for (uint8_t i = 0; i < 32; i++) {
    entry_t idt_entry = IDT_ENTRY((uint32_t) isr_stub_table[i], 0x8E);
    l_entries[i] = idt_entry;
  }

  __asm__ volatile("lidt %0" ::"m"(descriptor));
  __asm__ volatile("sti");

  printk("\nInterrupts", "Loaded IDT!");
}

}
