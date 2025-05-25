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

#ifndef __boot_interrupts_h
#define __boot_interrupts_h

#include <boot/gdt.h>
#include <common.h>
#include <stdint.h>

#define IDT_ENTRY(isr, attributes)                                            \
  {                                                                           \
    (isr & 0xffff),         /* isr_low */                                     \
    GDT_KERNEL_CODE_OFFSET, /* segment_selector */                            \
    0,                      /* reserved */                                    \
    attributes,             /* attributes */                                  \
    (isr >> 16)             /* isr_high */                                    \
  }

namespace Interrupts
{

typedef struct {
  uint16_t isr_low;
  uint16_t segment_selector;
  uint8_t reserved;
  uint8_t attributes;
  uint16_t isr_high;
} PACKED entry_t;

typedef struct {
  uint16_t limit; /* sizeof(IDT) - 1 */
  entry_t *ptr;   /* Address of IDT */
} PACKED descriptor_t;

void load_idt(void);
extern "C" NORETURN void exception_handler(void);

}

#endif
