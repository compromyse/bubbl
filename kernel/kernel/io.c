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

#include <kernel/io.h>
#include <stdint.h>

/* Sends a 8/16/32-bit value on a I/O location. Traditional names are outb,
 * outw and outl respectively. The a modifier enforces val to be placed in the
 * eax register before the asm command is issued and Nd allows for one-byte
 * constant values to be assembled as constants, freeing the edx register for
 * other cases. */

void
outb(uint16_t port, uint8_t val)
{
  __asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");

  /* There's an outb %al, $imm8 encoding, for compile-time constant port
   * numbers that fit in 8b. (N constraint). Wider immediate constants would be
   * truncated at assemble-time (e.g. "i" constraint). The  outb  %al, %dx
   * encoding is the only option for all other cases. %1 expands to %dx because
   * port  is a uint16_t.  %w1 could be used if we had the port number a wider
   * C type */
}

/* Receives a 8/16/32-bit value from an I/O location. Traditional names are
 * inb, inw and inl respectively. */

uint8_t
inb(uint16_t port)
{
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}
