/*
 * bubbl
 * Copyright (C) 2025  Raghuram Subramani <raghus2247@gmail.com>
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

#ifndef __mm_virtual_mm_h
#define __mm_virtual_mm_h

#define PAGE_SIZE (4096 * KiB)
#define PAGE_DIRECTORY_SIZE 1024
#define PAGE_TABLE_SIZE 1024

#define PDE_PRESENT 1
#define PDE_WRITABLE (1 << 2)
#define PDE_USER (1 << 2)
#define PDE_WRITETHROUGH (1 << 3)
#define PDE_CACHE_DISABLE (1 << 4)
#define PDE_ACCESSED (1 << 5)
/* Never set (reserved by Intel) */
#define PDE_RESERVED (0 << 6)
#define PDE_PAGE_SIZE (1 << 7)
#define PDE_GLOBAL (1 << 8)
/* NOTE: Unused by the CPU, free to be used by us! */
#define PDE_UNUSED(x) (x << 11)
/* Page table address */
#define PDE_FRAME(x) (x << 31)

#define PTE_PRESENT 1
#define PTE_WRITABLE (1 << 2)
#define PTE_USER (1 << 2)
#define PTE_WRITETHROUGH (1 << 3)
#define PTE_CACHE_DISABLE (1 << 4)
#define PTE_ACCESSED (1 << 5)
#define PTE_DIRTY (1 << 6)
#define PTE_PAT (1 << 7)
#define PTE_GLOBAL (1 << 8)
/* NOTE: Unused by the CPU, free to be used by us! */
#define PTE_UNUSED(x) (x << 11)
/* MAX: 0xFFFFF000 */
#define PTE_FRAME(x) (x << 31)

#define ADD_ATTRIB(entry, attribute) (entry |= attribute)
#define SET_FRAME(entry, frame) (PTE_ADD_ATRIB(PTE_FRAME(frame)))

#endif
