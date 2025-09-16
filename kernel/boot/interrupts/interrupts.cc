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

void
initialize()
{
  load_idt();
  printk("\ninterrupts", "IDT Loaded.");

  enable();
  printk("interrupts", "Initialized.");
}

void
enable(void)
{
  if (!idt_loaded()) {
    printk("interrupts", "Attempt to enable before IDT load.");
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
