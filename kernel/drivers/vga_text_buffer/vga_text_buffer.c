/*
 * CMOS
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

#include <libk/stdio.h>
#include <libk/string.h>

#include <kernel/io.h>

#include <drivers/vga_text_buffer.h>

static uint16_t *vga_text_buffer_buffer = (uint16_t *) 0xB8000;
static uint8_t vga_text_buffer_row = 0;
static uint8_t vga_text_buffer_column = 0;
static uint8_t vga_text_buffer_color;

static uint8_t
vga_entry_color(const vga_color fg, const vga_color bg)
{
  /*
   * bg   fg
   * 1110 0101
   */
  return bg << 4 | fg;
}

static uint16_t
vga_entry(const unsigned char character, const uint8_t color)
{
  /*
   * color     character
   * 1110 0101 1001 1010
   */
  return (uint16_t) color << 8 | (uint16_t) character;
}

static void
vga_text_buffer_write_entry_at(const char c,
                               const uint8_t color,
                               const uint8_t x,
                               const uint8_t y)
{
  size_t index = y * VGA_WIDTH + x;
  vga_text_buffer_buffer[index] = vga_entry(c, color);
}

void
vga_text_buffer_initialize(void)
{
  /*
   * Disable the cursor
   * TODO: Abstract this and all other cursor functionality out
   */
  outb(0x3D4, 0x0A);
  outb(0x3D5, 0x20);

  vga_text_buffer_color
      = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

  for (uint8_t y = 0; y < VGA_HEIGHT; y++)
    for (uint8_t x = 0; x < VGA_WIDTH; x++)
      vga_text_buffer_write_entry_at(' ', vga_text_buffer_color, x, y);
}

void
vga_text_buffer_write_char(const char c)
{
  if (c == '\n') {
    vga_text_buffer_row++;
    vga_text_buffer_column = 0;
  } else {
    vga_text_buffer_write_entry_at(
        c, vga_text_buffer_color, vga_text_buffer_column, vga_text_buffer_row);

    if (++vga_text_buffer_column == VGA_WIDTH) {
      vga_text_buffer_column = 0;
      if (++vga_text_buffer_row == VGA_HEIGHT)
        vga_text_buffer_row = 0;
    }
  }
}

void
vga_text_buffer_write_string(const char *string)
{
  size_t size = strlen(string);
  for (size_t i = 0; i < size; i++)
    vga_text_buffer_write_char(string[i]);
}

void
vga_text_buffer_printf(const char *string, ...)
{
  /* TODO: Dynamic Memory Allocation */
  char str[256];

  va_list ap;
  va_start(ap, string);
  vsnprintf(str, sizeof(str), string, ap);
  va_end(ap);

  vga_text_buffer_write_string(str);
}
