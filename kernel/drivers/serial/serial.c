/*
 * CMOS
 * Copyright (C) 2024-2025 Raghuram Subramani <raghus2247@gmail.com>
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

#include <libk/string.h>

#include <kernel/io.h>

#include <drivers/serial.h>

/* Implementation adapted from
 * https://wiki.osdev.org/Inline_Assembly/Examples */

void
serial_initialize(void)
{
  outb(PORT + 1, 0x00); // Disable all interrupts
  outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
  outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
  outb(PORT + 1, 0x00); //                  (hi byte)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
  outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
  outb(PORT + 4, 0x1E); // Set in loopback mode, test the serial chip
  outb(PORT + 0, 0xAE); // Test serial chip (send byte 0xAE and check if serial
                        // returns same byte)

  // TODO: Check if serial is faulty (i.e: not same byte as sent)
  /* if (inb(PORT + 0) != 0xAE) {
    return 1;
  } */

  // If serial is not faulty set it in normal operation mode
  // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
  outb(PORT + 4, 0x0F);
}

static int
is_transmit_empty()
{
  return inb(PORT + 5) & 0x20;
}

void
serial_write_char(char a)
{
  while (is_transmit_empty() == 0)
    ;

  outb(PORT, a);
}

void
serial_write_string(char *string)
{
  size_t size = strlen(string);
  for (size_t i = 0; i < size; i++)
    serial_write_char(string[i]);
}
