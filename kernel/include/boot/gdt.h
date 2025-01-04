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

#ifndef __boot_gdt_h
#define __boot_gdt_h

#include <stdint.h>

/* Access Flags:
 * 7 PRESENT
 * 6 PRIVILEGE
 * 5 PRIVILEGE
 * 4 TYPE
 * 3 EXECUTABLE
 * 2 DIRECTION/CONFORMING
 * 1 READABLE/WRITABLE
 * 0 ACCESSED
 */

#define PRESENT(x) (x << 7)
#define PRIVILEGE(x) (x << 5)
#define TYPE(x) (x << 4)
#define EXECUTABLE(x) (x << 3)
#define DIRECTION_CONFORMING(x) (x << 2)
#define READABLE_WRITABLE(x) (x << 1)
#define ACCESSED(x) x

#define CODE_SEGMENT_ACCESS_FLAGS(privilege)                                  \
  PRESENT(1) | PRIVILEGE(privilege) | TYPE(1) | EXECUTABLE(1)                 \
      | DIRECTION_CONFORMING(0) | READABLE_WRITABLE(1) | ACCESSED(0)

#define DATA_SEGMENT_ACCESS_FLAGS(privilege)                                  \
  PRESENT(1) | PRIVILEGE(privilege) | TYPE(1) | EXECUTABLE(0)                 \
      | DIRECTION_CONFORMING(0) | READABLE_WRITABLE(1) | ACCESSED(0)

#define KERNEL_CODE_SEGMENT_ACCESS_FLAGS CODE_SEGMENT_ACCESS_FLAGS(0)
#define KERNEL_DATA_SEGMENT_ACCESS_FLAGS DATA_SEGMENT_ACCESS_FLAGS(0)

#define USER_CODE_SEGMENT_ACCESS_FLAGS CODE_SEGMENT_ACCESS_FLAGS(3)
#define USER_DATA_SEGMENT_ACCESS_FLAGS DATA_SEGMENT_ACCESS_FLAGS(3)

/* Other Flags:
 * 3 GRANULARITY
 * 2 SIZE
 * 1 LONGMODE
 * 0 RESERVED
 */

#define GRANULARITY (1 << 3)
#define SIZE (1 << 2)
#define LONGMODE (0 << 1)
#define RESERVED 0

#define FLAGS (GRANULARITY | SIZE | LONGMODE | RESERVED)

#define FLAGS_LIMIT_HIGH(flags, limit_high) ((flags << 4) | limit_high)

/* GDT Entry:
 * BASE  FLAGS  LIMIT  ACCESS_FLAGS  BASE  BASE  LIMIT
 * 8bit  4bit   4bit   8bit          8bit  16bit 16bit
 */

#define GDT_ENTRY(base, limit, access_flags, flags)                           \
  {                                                                           \
    (limit & 0xffff),          /* limit_low */                                \
        (base & 0xffff),       /* base_low */                                 \
        ((base >> 16) & 0xff), /* base_mid */                                 \
        access_flags,          /* access_flags */                             \
        FLAGS_LIMIT_HIGH(flags,                                               \
                         ((limit >> 16) & 0xff)), /* flags_limit_high */      \
        ((base >> 24) & 0xff)                     /* base_high */             \
  }

typedef struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access_flags;
  uint8_t flags_limit_high;
  uint8_t base_high;
} __attribute__((packed)) GDT_entry;

typedef struct {
  uint16_t limit; /* sizeof(GDT) - 1 */
  GDT_entry *ptr; /* Address of GDT */
} __attribute__((packed)) GDT_descriptor;

extern void _GDT_flush(GDT_descriptor *);
void GDT_load(void);

#endif
