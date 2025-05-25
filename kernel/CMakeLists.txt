cmake_minimum_required(VERSION 3.21)
project(kernel C ASM_NASM)

set(SRC
  boot/gdt/gdt.cc
  boot/gdt/gdt.s
  boot/init/boot.s
  boot/init/crti.s
  boot/init/crtn.s
  boot/interrupts/exceptions.cc
  boot/interrupts/idt.cc
  boot/interrupts/isr.s
  drivers/serial.cc
  drivers/vga_text_buffer.cc
  kernel/halt.cc
  kernel/io.cc
  kernel/kernel.cc
  kernel/spinlock.cc
  kernel/stack_smashing_protector.cc
  libk/liballoc.cc
  libk/memset.cc
  libk/printf.cc
  libk/printk.cc
  libk/strlen.cc
  mm/memory_map.cc
  mm/physical_mm/bitmap.cc
  mm/physical_mm/physical_mm.cc
  mm/virtual_mm/page_table_allocator.cc
  mm/virtual_mm/pages.cc
  mm/virtual_mm/virtual_mm.cc
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
