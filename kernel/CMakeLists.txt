cmake_minimum_required(VERSION 3.21)
project(kernel C ASM)

set(KERNEL_SRC
  kernel/kernel.c
  arch/x86/init/boot.s
)

add_executable(kernel ${KERNEL_SRC})

set(C_COMPILE_OPTIONS
  -ffreestanding
  -O2
  -Wall
  -Wextra
  -pedantic
)

target_compile_options(kernel PRIVATE 
  $<$<COMPILE_LANGUAGE:C>: ${C_COMPILE_OPTIONS}>
)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/arch/x86/link.ld")
set(LINKER_FLAGS
  -T ${LINKER_SCRIPT}
  -nostdlib
)
target_link_options(kernel PRIVATE ${LINKER_FLAGS})
