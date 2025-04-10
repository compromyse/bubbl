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

/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
  /* It used to be universally recommended to use 1M as a start offset,
	as it was effectively guaranteed to be available under BIOS systems.
	However, UEFI has made things more complicated, and experimental data
	strongly suggests that 2M is a safer place to load. In 2016, a new
	feature was introduced to the multiboot2 spec to inform bootloaders
	that a kernel can be loaded anywhere within a range of addresses and
	will be able to relocate itself to run from such a loader-selected
	address, in order to give the loader freedom in selecting a span of
	memory which is verified to be available by the firmware, in order to
	work around this issue. This does not use that feature, so 2M was
	chosen as a safer option than the traditional 1M. */
  . = 2M;
  kernel_start = .;

  /* First put the multiboot header, as it is required to be put very early
	in the image or the bootloader won't recognize the file format.
	Next we'll put the .text section. */
  .text : ALIGN(4K)
  {
    *(.multiboot)
    *(.text)

    *(.init)
    *(.fini)

    *(.ctors)
    *(.dtors)
  }

  /* Read-only data. */
  .rodata : ALIGN(4K)
  {
    *(.rodata*)
  }

  /* Read-write data (initialized) */
  .data : ALIGN(4K)
  {
    *(.data*)
  }

  /* Read-write data (uninitialized) and stack */
  .bss : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }

  .debug : ALIGN(4K)
  {
    *(.comment)
    *(.debug*)
  }

  .eh_frame : ALIGN(4K)
  {
    *(.eh_frame)
  }

  .symtab : ALIGN(4K)
  {
    *(.symtab)
  }

  .shstrtab : ALIGN(4K)
  {
    *(.shstrtab)
  }

  .strtab : ALIGN(4K)
  {
    *(.strtab)
  }


  kernel_end = .;
}
