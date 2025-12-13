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

#ifndef __drivers_vga_text_buffer_h
#define __drivers_vga_text_buffer_h

/*
 * TODO:
 * Implement scrolling
 */

#include <stdbool.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

/*
 * bg   fg
 * 1110 0101
 */
#define VGA_ENTRY_COLOR(fg, bg) (bg << 4 | fg)

/*
 * color     character
 * 1110 0101 1001 1010
 */
#define VGA_ENTRY(character, color)                                           \
  ((uint16_t) color << 8 | (uint16_t) character)

/* Hardware text mode color constants. */
typedef enum {
  COLOR_BLACK = 0,
  COLOR_BLUE = 1,
  COLOR_GREEN = 2,
  COLOR_CYAN = 3,
  COLOR_RED = 4,
  COLOR_MAGENTA = 5,
  COLOR_BROWN = 6,
  COLOR_LIGHT_GREY = 7,
  COLOR_DARK_GREY = 8,
  COLOR_LIGHT_BLUE = 9,
  COLOR_LIGHT_GREEN = 10,
  COLOR_LIGHT_CYAN = 11,
  COLOR_LIGHT_RED = 12,
  COLOR_LIGHT_MAGENTA = 13,
  COLOR_LIGHT_BROWN = 14,
  COLOR_WHITE = 15
} vgatb_colors;

void vgatb_initialize(void);
void vgatb_write_char(const char);
void vgatb_write_string(const char *string);
void vgatb_printf(const char *string, ...);

#endif
