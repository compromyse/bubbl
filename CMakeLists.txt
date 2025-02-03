set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
# The checks don't work on cross compilers
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

cmake_minimum_required(VERSION 3.21)
project(bubbl)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(run)

add_subdirectory(kernel)

get_target_property(KERNEL_BINARY_DIR kernel BINARY_DIR)
get_target_property(KERNEL_SOURCE kernel SOURCE_DIR)

add_custom_target(iso
  rm -rf ${CMAKE_BINARY_DIR}/iso &&
  mkdir -p ${CMAKE_BINARY_DIR}/iso/boot/grub &&
  cp ${KERNEL_BINARY_DIR}/kernel ${CMAKE_BINARY_DIR}/iso/boot &&
  cp ${KERNEL_SOURCE}/boot/grub.cfg ${CMAKE_BINARY_DIR}/iso/boot/grub &&
  grub-mkrescue -o ${CMAKE_BINARY_DIR}/bubbl.iso ${CMAKE_BINARY_DIR}/iso

  DEPENDS kernel
)

set_property(
  TARGET iso
  APPEND
  PROPERTY ADDITIONAL_CLEAN_FILES iso
  PROPERTY ADDITIONAL_CLEAN_FILES bubbl.iso
)
