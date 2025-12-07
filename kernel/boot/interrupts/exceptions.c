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
#include <libk/stdio.h>
#include <stdbool.h>

void
exception_handler(int irq_number)
{
  printk("interrupts", "Interrupt: %d", irq_number);

  ASSERT_NOT_REACHED();
  while (true)
    __asm__ volatile("cli; hlt");
}
