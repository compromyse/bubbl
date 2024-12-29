# CMakeLists.txt

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

cmake_minimum_required(VERSION 3.21)
project(CMOS C ASM)

set(KERNEL_SRC
  kernel/kernel.c
  arch/x86/init/boot.s
)

add_executable(cmos.bin ${KERNEL_SRC})

set(C_COMPILE_OPTIONS
  -ffreestanding
  -O2
  -Wall
  -Wextra
  -pedantic
)

target_compile_options(cmos.bin PRIVATE 
  $<$<COMPILE_LANGUAGE:C>: ${C_COMPILE_OPTIONS}>
)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/arch/x86/link.ld")
set(LINKER_FLAGS
  -T ${LINKER_SCRIPT}
  -nostdlib
)
target_link_options(cmos.bin PRIVATE ${LINKER_FLAGS})
