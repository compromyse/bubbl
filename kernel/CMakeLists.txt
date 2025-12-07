cmake_minimum_required(VERSION 3.21)
project(kernel C ASM_NASM)

set(SRC
  boot/gdt/gdt.c
  boot/gdt/gdt.s
  boot/init/boot.s
  boot/init/crti.s
  boot/init/crtn.s
  boot/interrupts/exceptions.c
  boot/interrupts/idt.c
  boot/interrupts/interrupts.c
  boot/interrupts/isr.s
  drivers/serial.c
  drivers/vga_text_buffer.c
  kernel/halt.c
  kernel/io.c
  kernel/kernel.c
  kernel/spinlock.c
  kernel/stack_smashing_protector.c
  libk/liballoc.c
  libk/memset.c
  libk/printf.c
  libk/printk.c
  libk/strlen.c
  mm/memory_map.c
  mm/physical_mm/bitmap.c
  mm/physical_mm/physical_mm.c
  mm/virtual_mm/page_table_allocator.c
  mm/virtual_mm/pages.c
  mm/virtual_mm/virtual_mm.c
)

add_executable(kernel ${SRC})
target_include_directories(kernel PRIVATE include)

set(C_COMPILE_OPTIONS
  -march=i386

  -ffreestanding

  -fstack-protector
  -fstack-protector-all

  # -O3
  -O0

  -Wall
  -Wextra
  -pedantic
  # -Werror

  # TODO: This doesn't actually work right now because of the linker script :')
  -g
)

set(CXX_COMPILE_OPTIONS
  -std=c++98

  -fno-exceptions
  -fno-rtti

  -Wno-write-strings
  -Wno-missing-field-initializers
  -Wno-c++11-long-long
  -Wno-c99-extensions
  -Wno-c++14-binary-literal
)

target_compile_options(kernel PRIVATE 
  $<$<COMPILE_LANGUAGE:C>: ${C_COMPILE_OPTIONS}>
  $<$<COMPILE_LANGUAGE:CXX>: ${C_COMPILE_OPTIONS} ${CXX_COMPILE_OPTIONS}>
)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/boot/linker.ld")
target_link_options(kernel PRIVATE
  -T ${LINKER_SCRIPT}
  -nostdlib
)
