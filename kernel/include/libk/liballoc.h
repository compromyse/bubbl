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

#ifndef __libk_liballoc_h
#define __libk_liballoc_h

#include <stddef.h>

/** This is a boundary tag which is prepended to the
 * page or section of a page which we have allocated. It is
 * used to identify valid memory blocks that the
 * application is trying to free.
 */
struct boundary_tag {
  unsigned int magic;     //< It's a kind of ...
  unsigned int size;      //< Requested size.
  unsigned int real_size; //< Actual size.
  int index;              //< Location in the page table.

  struct boundary_tag *split_left;  //< Linked-list info for broken pages.
  struct boundary_tag *split_right; //< The same.

  struct boundary_tag *next; //< Linked list info.
  struct boundary_tag *prev; //< Linked list info.
};

void *kmalloc(size_t);
void *krealloc(void *, size_t);
void *kcalloc(size_t, size_t);
void kfree(void *);

#endif
