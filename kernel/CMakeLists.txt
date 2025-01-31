cmake_minimum_required(VERSION 3.21)
project(kernel C ASM)

set(SRC
  boot/gdt/gdt.cc
  boot/gdt/gdt.s
  boot/init/boot.s

  drivers/serial.cc
  drivers/vga_text_buffer.cc

  kernel/halt.c
  kernel/io.c
  kernel/kernel.cc
  kernel/spinlock.cc
  kernel/stack_smashing_protector.c

  libk/printf.c
  libk/printk.cc
  libk/strlen.c
  libk/kmalloc.cc

  mm/memory_map.c
  mm/physical_mm/memory_map.cc
  mm/physical_mm/physical_mm.cc
  mm/virtual_mm/virtual_mm.cc
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
)

target_compile_options(kernel PRIVATE 
  $<$<COMPILE_LANGUAGE:C>: ${C_COMPILE_OPTIONS}>
  $<$<COMPILE_LANGUAGE:CXX>: ${C_COMPILE_OPTIONS} ${CXX_COMPILE_OPTIONS}>
)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/boot/linker.ld")
set(LINKER_FLAGS
  -T ${LINKER_SCRIPT}
  -nostdlib
)

execute_process(COMMAND ${CMAKE_CXX_COMPILER}
  -print-file-name=crtbegin.o
  OUTPUT_VARIABLE CRTBEGIN_O
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(COMMAND ${CMAKE_CXX_COMPILER}
  -print-file-name=crtend.o
  OUTPUT_VARIABLE CRTEND_O
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

add_library(crti OBJECT boot/init/crti.s)
add_library(crtn OBJECT boot/init/crtn.s)
add_dependencies(kernel crti crtn)

get_target_property(CRTI_SRC crti SOURCES)
get_target_property(CRTN_SRC crtn SOURCES)
get_target_property(CRTI_OUT crti BINARY_DIR)
get_target_property(CRTN_OUT crtn BINARY_DIR)

set(CRTI_O "${CRTI_OUT}/CMakeFiles/crti.dir/${CRTI_SRC}.o")
set(CRTN_O "${CRTN_OUT}/CMakeFiles/crtn.dir/${CRTN_SRC}.o")

# FIXME: This isn't a good way of setting the link order.
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_COMPILER} <CMAKE_CXX_LINK_FLAGS> <FLAGS> <LINK_FLAGS> \
  ${CRTI_O} \
  ${CRTBEGIN_O} \
  <OBJECTS> \
  ${CRTEND_O} \
  ${CRTN_O} \
  -o <TARGET> <LINK_LIBRARIES>"
)

target_link_options(kernel PRIVATE ${LINKER_FLAGS})
target_link_libraries(kernel PRIVATE gcc)
