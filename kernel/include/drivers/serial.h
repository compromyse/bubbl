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

#ifndef __drivers_serial_h
#define __drivers_serial_h

#include <stdbool.h>

#define PORT 0x3f8 // COM1

#ifdef __cplusplus
extern "C" {
#endif

bool serial_initialize(void);
void serial_write_char(const char chr);
void serial_write_string(const char *string);

#ifdef __cplusplus
}
#endif

#endif
