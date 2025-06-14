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
#include <kernel/spinlock.h>

void
Spinlock::acquire(void)
{
  Interrupts::disable();
  while (!__sync_bool_compare_and_swap(&m_lock, 0, 1))
    while (m_lock)
      __asm__ volatile("rep; nop");
}

void
Spinlock::release(void)
{
  __sync_bool_compare_and_swap(&m_lock, 1, 0);
  if (Interrupts::idt_loaded())
    Interrupts::enable();
}
