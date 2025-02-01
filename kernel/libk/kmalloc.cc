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

#include <libk/kmalloc.h>
#include <libk/stdio.h>
#include <mm/virtual_mm.h>
#include <stddef.h>
#include <stdint.h>

/* TODO: Kmalloc must have space for a page table *at all times*. */

bool initialized = false;

bool
kmalloc_initialized(void)
{
  return initialized;
}

#define LIBALLOC_MAGIC 0xc001c0de
#define MAXCOMPLETE 5
#define MAXEXP 32
#define MINEXP 8

#define MODE_BEST 0
#define MODE_INSTANT 1

#define MODE MODE_BEST

#ifdef DEBUG
#include <stdio.h>
#endif

struct boundary_tag *l_freePages[MAXEXP]; //< Allowing for 2^MAXEXP blocks
int l_completePages[MAXEXP];              //< Allowing for 2^MAXEXP blocks

#ifdef DEBUG
unsigned int l_allocated = 0; //< The real amount of memory allocated.
unsigned int l_inuse = 0;     //< The amount of memory in use (malloc'ed).
#endif

static unsigned int l_initialized = 0; //< Flag to indicate initialization.
static unsigned int l_pageSize = 4096; //< Individual page size
static unsigned int l_pageCount = 1;   //< Minimum number of pages to allocate.

static inline int
getexp(unsigned int size)
{
  if (size < (1 << MINEXP)) {
    return -1; // Smaller than the quantum.
  }

  int shift = MINEXP;

  while (shift < MAXEXP) {
    if ((1 << shift) > size)
      break;
    shift += 1;
  }

  return shift - 1;
}

static inline void
insert_tag(struct boundary_tag *tag, int index)
{
  int realIndex;

  if (index < 0) {
    realIndex = getexp(tag->real_size - sizeof(struct boundary_tag));
    if (realIndex < MINEXP)
      realIndex = MINEXP;
  } else
    realIndex = index;

  tag->index = realIndex;

  if (l_freePages[realIndex] != NULL) {
    l_freePages[realIndex]->prev = tag;
    tag->next = l_freePages[realIndex];
  }

  l_freePages[realIndex] = tag;
}

static inline void
remove_tag(struct boundary_tag *tag)
{
  if (l_freePages[tag->index] == tag)
    l_freePages[tag->index] = tag->next;

  if (tag->prev != NULL)
    tag->prev->next = tag->next;
  if (tag->next != NULL)
    tag->next->prev = tag->prev;

  tag->next = NULL;
  tag->prev = NULL;
  tag->index = -1;
}

static inline struct boundary_tag *
split_tag(struct boundary_tag *tag)
{
  unsigned int remainder
      = tag->real_size - sizeof(struct boundary_tag) - tag->size;

  struct boundary_tag *new_tag
      = (struct boundary_tag *) ((unsigned int) tag
                                 + sizeof(struct boundary_tag) + tag->size);

  new_tag->magic = LIBALLOC_MAGIC;
  new_tag->real_size = remainder;

  new_tag->next = NULL;
  new_tag->prev = NULL;

  new_tag->split_left = tag;
  new_tag->split_right = tag->split_right;

  if (new_tag->split_right != NULL)
    new_tag->split_right->split_left = new_tag;
  tag->split_right = new_tag;

  tag->real_size -= new_tag->real_size;

  insert_tag(new_tag, -1);

  return new_tag;
}

static struct boundary_tag *
allocate_new_tag(unsigned int size)
{
  unsigned int pages;
  unsigned int usage;
  struct boundary_tag *tag;

  // This is how much space is required.
  usage = size + sizeof(struct boundary_tag);

  // Perfect amount of space
  pages = usage / l_pageSize;
  if ((usage % l_pageSize) != 0)
    pages += 1;

  // Make sure it's >= the minimum size.
  if (pages < l_pageCount)
    pages = l_pageCount;

  tag = (struct boundary_tag *) liballoc_alloc(pages);

  if (tag == NULL)
    return NULL; // uh oh, we ran out of memory.

  tag->magic = LIBALLOC_MAGIC;
  tag->size = size;
  tag->real_size = pages * l_pageSize;
  tag->index = -1;

  tag->next = NULL;
  tag->prev = NULL;
  tag->split_left = NULL;
  tag->split_right = NULL;

  return tag;
}

void *
kmalloc(size_t size)
{
  int index;
  void *ptr;
  struct boundary_tag *tag = NULL;

  // liballoc_lock();

  if (l_initialized == 0) {
    for (index = 0; index < MAXEXP; index++) {
      l_freePages[index] = NULL;
      l_completePages[index] = 0;
    }
    l_initialized = 1;
  }

  index = getexp(size) + MODE;
  if (index < MINEXP)
    index = MINEXP;

  // Find one big enough.
  tag = l_freePages[index]; // Start at the front of the list.
  while (tag != NULL) {
    // If there's enough space in this tag.
    if ((tag->real_size - sizeof(struct boundary_tag))
        >= (size + sizeof(struct boundary_tag))) {
      break;
    }

    tag = tag->next;
  }

  // No page found. Make one.
  if (tag == NULL) {
    if ((tag = allocate_new_tag(size)) == NULL) {
      // liballoc_unlock();
      return NULL;
    }

    index = getexp(tag->real_size - sizeof(struct boundary_tag));
  } else {
    remove_tag(tag);

    if ((tag->split_left == NULL) && (tag->split_right == NULL))
      l_completePages[index] -= 1;
  }

  // We have a free page.  Remove it from the free pages list.

  tag->size = size;

  // Removed... see if we can re-use the excess space.

  unsigned int remainder
      = tag->real_size - size
        - sizeof(struct boundary_tag) * 2; // Support a new tag + remainder

  if (((int) (remainder)
       > 0) /*&& ( (tag->real_size - remainder) >= (1<<MINEXP))*/) {
    int childIndex = getexp(remainder);

    if (childIndex >= 0) {
      struct boundary_tag *new_tag = split_tag(tag);
      (void) new_tag;
    }
  }

  ptr = (void *) ((unsigned int) tag + sizeof(struct boundary_tag));
  // liballoc_unlock();
  return ptr;
}
