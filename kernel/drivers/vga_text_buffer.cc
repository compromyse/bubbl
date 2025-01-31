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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <common.h>

#include <libk/stdio.h>
#include <libk/string.h>

#include <kernel/io.h>

#include <drivers/vga_text_buffer.h>

namespace VGATextBuffer
{

static uint16_t *buffer = (uint16_t *) 0xB8000;
static uint8_t row = 0;
static uint8_t column = 0;
static uint8_t color = vga_entry_color(VGATextBuffer::COLOR_LIGHT_GREY,
                                       VGATextBuffer::COLOR_BLACK);

ALWAYS_INLINE static void
write_entry_at(const char c,
               const uint8_t color,
               const uint8_t x,
               const uint8_t y)
{
  size_t index = y * VGA_WIDTH + x;
  buffer[index] = vga_entry(c, color);
}

void
initialize(void)
{
  /*
   * Disable the cursor
   * TODO: Abstract this and all other cursor functionality out
   */
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);

  for (uint8_t y = 0; y < VGA_HEIGHT; y++)
    for (uint8_t x = 0; x < VGA_WIDTH; x++)
      write_entry_at(' ', color, x, y);
}

void
write_char(const char c)
{
  if (c == '\n') {
    row++;
    column = 0;
  } else {
    write_entry_at(c, color, column, row);

    if (++column == VGA_WIDTH) {
      column = 0;
      if (++row == VGA_HEIGHT)
        row = 0;
    }
  }
}

void
write_string(const char *string)
{
  size_t size = strlen(string);
  for (size_t i = 0; i < size; i++)
    write_char(string[i]);
}

void
printf(const char *string, ...)
{
  /* TODO: Dynamic Memory Allocation */
  char str[256];

  va_list ap;
  va_start(ap, string);
  vsnprintf(str, sizeof(str), string, ap);
  va_end(ap);

  write_string(str);
}

}
