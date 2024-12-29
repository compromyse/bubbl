set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

cmake_minimum_required(VERSION 3.21)
project(CMOS)

add_subdirectory(kernel)

set(QEMU_SYSTEM_CMD qemu-system-i386)

add_custom_target(run
  ${QEMU_SYSTEM_CMD} -kernel kernel/kernel
)
