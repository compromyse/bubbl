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

#include <boot/gdt.h>
#include <kernel/io.h>

namespace GDT
{

entry_t l_entries[] = {
  /* NULL Descriptor */
  GDT_ENTRY(0, 0, 0, 0),

  /* Kernel Mode Code Segment */
  GDT_ENTRY(0, 0xfffff, KERNEL_CODE_SEGMENT_ACCESS_FLAGS, FLAGS),

  /* Kernel Mode Data Segment */
  GDT_ENTRY(0, 0xfffff, KERNEL_DATA_SEGMENT_ACCESS_FLAGS, FLAGS),

  /* User Mode Code Segment */
  // GDT_ENTRY(0, 0xfffff, USER_CODE_SEGMENT_ACCESS_FLAGS, FLAGS),

  /* User Mode Data Segment */
  // GDT_ENTRY(0, 0xfffff, USER_DATA_SEGMENT_ACCESS_FLAGS, FLAGS)

  /* TODO: TSS? */
  /* TODO: LDT? */
};

descriptor_t descriptor = { sizeof(l_entries) - 1, l_entries };

void
load(void)
{
  _GDT_flush(&descriptor);
}

}
