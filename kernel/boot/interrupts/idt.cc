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
#include <common.h>
#include <kernel/halt.h>
#include <kernel/io.h>
#include <libk/stdio.h>

namespace Interrupts
{

extern "C" void *isr_stub_table[];

entry_t l_entries[256];
descriptor_t descriptor = { sizeof(l_entries) - 1, l_entries };
static bool l_idt_loaded = false;

bool
idt_loaded(void)
{
  return l_idt_loaded;
}

void
initialize()
{
  load_idt();
  printk("\nInterrupts", "IDT Loaded.");
  enable();
  printk("Interrupts", "Initialized.");
}

void
load_idt(void)
{
  for (uint16_t i = 0; i < 256; i++)
    l_entries[i] = (entry_t) { 0 };

  /* The first 32 entries are exceptions */
  for (uint8_t i = 0; i < 32; i++)
    l_entries[i] = (entry_t) IDT_ENTRY((uint32_t) isr_stub_table[i],
                                       IDT_PRESENT | IDT_KERNEL_PRIVILEGE_LEVEL
                                           | IDT_32BIT_INTERRUPT_GATE);

  __asm__ volatile("lidt %0" ::"m"(descriptor));

  l_idt_loaded = true;
}

void
enable(void)
{
  if (!l_idt_loaded) {
    printk("Interrupts", "Attempt to enable before IDT load.");
    ASSERT_NOT_REACHED();
  }

  __asm__ volatile("sti");
  // printk("\ninterrupts", "Enabled.");
}

void
disable(void)
{
  __asm__ volatile("cli");
  // printk("\ninterrupts", "Disabled.");
}

}
