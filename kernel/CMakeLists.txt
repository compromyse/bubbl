cmake_minimum_required(VERSION 3.21)
project(kernel C ASM_NASM)

set(SRC
  boot/init/crti.s
  
  boot/gdt/gdt.cc
  boot/gdt/gdt.s
  boot/init/boot.s

  drivers/serial.cc
  drivers/vga_text_buffer.cc

  kernel/halt.cc
  kernel/io.cc
  kernel/kernel.cc
  kernel/spinlock.cc
  kernel/stack_smashing_protector.cc

  libk/printf.cc
  libk/printk.cc
  libk/strlen.cc
  libk/liballoc.cc

  mm/memory_map.cc
  mm/physical_mm/bitmap.cc
  mm/physical_mm/physical_mm.cc
  mm/virtual_mm/virtual_mm.cc
  mm/virtual_mm/pages.cc

  boot/init/crtn.s
)

add_executable(kernel ${SRC})
target_include_directories(kernel PRIVATE include)

set(C_COMPILE_OPTIONS
  -ffreestanding

  -fstack-protector
  -fstack-protector-all

  # -O3

  -Wall
  -Wextra
  -pedantic
  # -Werror

  # TODO: This doesn't actually work right now because of the linker script :')
  -g
)

set(CXX_COMPILE_OPTIONS
  -fno-exceptions
  -fno-rtti
  -std=c++98

  -Wno-write-strings
  -Wno-missing-field-initializers
  -Wno-c++11-long-long
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
