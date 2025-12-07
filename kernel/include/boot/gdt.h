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

#ifndef __boot_gdt_h
#define __boot_gdt_h

#include <common.h>
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

#define GDT_PRESENT(x) (x << 7)
#define GDT_PRIVILEGE(x) (x << 5)
#define GDT_TYPE(x) (x << 4)
#define GDT_EXECUTABLE(x) (x << 3)
#define GDT_DIRECTION_CONFORMING(x) (x << 2)
#define GDT_READABLE_WRITABLE(x) (x << 1)
#define GDT_ACCESSED(x) x

#define GDT_CODE_SEGMENT_ACCESS_FLAGS(privilege)                              \
  GDT_PRESENT(1) | GDT_PRIVILEGE(privilege) | GDT_TYPE(1) | GDT_EXECUTABLE(1) \
      | GDT_DIRECTION_CONFORMING(0) | GDT_READABLE_WRITABLE(1)                \
      | GDT_ACCESSED(0)

#define GDT_DATA_SEGMENT_ACCESS_FLAGS(privilege)                              \
  GDT_PRESENT(1) | GDT_PRIVILEGE(privilege) | GDT_TYPE(1) | GDT_EXECUTABLE(0) \
      | GDT_DIRECTION_CONFORMING(0) | GDT_READABLE_WRITABLE(1)                \
      | GDT_ACCESSED(0)

#define GDT_KERNEL_CODE_SEGMENT_ACCESS_FLAGS GDT_CODE_SEGMENT_ACCESS_FLAGS(0)
#define GDT_KERNEL_DATA_SEGMENT_ACCESS_FLAGS GDT_DATA_SEGMENT_ACCESS_FLAGS(0)

#define GDT_USER_CODE_SEGMENT_ACCESS_FLAGS GDT_CODE_SEGMENT_ACCESS_FLAGS(3)
#define GDT_USER_DATA_SEGMENT_ACCESS_FLAGS GDT_DATA_SEGMENT_ACCESS_FLAGS(3)

/* Other Flags:
 * 3 GRANULARITY
 * 2 SIZE
 * 1 LONGMODE
 * 0 RESERVED
 */

#define GDT_GRANULARITY (1 << 3)
#define GDT_SIZE (1 << 2)
#define GDT_LONGMODE (0 << 1)
#define GDT_RESERVED 0

#define GDT_FLAGS (GDT_GRANULARITY | GDT_SIZE | GDT_LONGMODE | GDT_RESERVED)

#define GDT_FLAGS_LIMIT_HIGH(flags, limit_high) ((flags << 4) | limit_high)

/* GDT Entry:
 * BASE  FLAGS  LIMIT  ACCESS_FLAGS  BASE  BASE  LIMIT
 * 8bit  4bit   4bit   8bit          8bit  16bit 16bit
 */

#define GDT_ENTRY(base, limit, access_flags, flags)                           \
  {                                                                           \
    (limit & 0xffff),      /* limit_low */                                    \
    (base & 0xffff),       /* base_low */                                     \
    ((base >> 16) & 0xff), /* base_mid */                                     \
    access_flags,          /* access_flags */                                 \
    GDT_FLAGS_LIMIT_HIGH(flags,                                               \
                         ((limit >> 16) & 0xff)), /* flags_limit_high */      \
    ((base >> 24) & 0xff)                         /* base_high */             \
  }

#define GDT_KERNEL_CODE_OFFSET 0x8
#define GDT_KERNEL_DATA_OFFSET 0x10

typedef struct {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access_flags;
  uint8_t flags_limit_high;
  uint8_t base_high;
} PACKED gdt_entry_t;

typedef struct {
  uint16_t limit;   /* sizeof(GDT) - 1 */
  gdt_entry_t *ptr; /* Address of GDT */
} PACKED gdt_descriptor_t;

void _GDT_flush(gdt_descriptor_t *descriptor);

void gdt_initialize(void);
void gdt_load(void);

#endif
