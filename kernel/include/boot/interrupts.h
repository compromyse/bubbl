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
#include <stdbool.h>
#include <stdint.h>

#define IDT_ENTRY(isr, attributes)                                            \
  {                                                                           \
    (isr & 0xffff),         /* isr_low */                                     \
    GDT_KERNEL_CODE_OFFSET, /* segment_selector */                            \
    0,                      /* reserved */                                    \
    attributes,             /* attributes */                                  \
    (isr >> 16)             /* isr_high */                                    \
  }

/* Attributes */
#define IDT_PRESENT (1 << 7)
#define IDT_KERNEL_PRIVILEGE_LEVEL (0)
#define IDT_USER_PRIVILEGE_LEVEL (3 << 5)
#define IDT_TASK_GATE (0x05)
#define IDT_16BIT_INTERRUPT_GATE (0x06)
#define IDT_16BIT_TRAP_GATE (0x07)
#define IDT_32BIT_INTERRUPT_GATE (0x0e)
#define IDT_32BIT_TRAP_GATE (0x0f)

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

/* Simply loads IDT and enables interrupts */
void interrupts_initialize(void);
void interrupts_enable(void);
void interrupts_disable(void);

/* IDT */
void idt_load(void);
bool idt_loaded(void);

NORETURN void exception_handler(int irq_number);

#endif
