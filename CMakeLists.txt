set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

cmake_minimum_required(VERSION 3.21)
project(bubbl)

add_subdirectory(kernel)

get_target_property(KERNEL_BINARY_DIR kernel BINARY_DIR)
get_target_property(KERNEL_SOURCE kernel SOURCE_DIR)

set(BOCHS_CMD bochs)
set(QEMU_SYSTEM_CMD qemu-system-i386)

set(QEMU_ARGUMENTS
  -cdrom ${CMAKE_BINARY_DIR}/bubbl.iso
  -device isa-debug-exit,iobase=0xf4,iosize=0x04
  -m 32M
  -smp 1
)

set(IGNORE_EXIT || true)

add_custom_target(iso
  rm -rf ${CMAKE_BINARY_DIR}/iso &&
  mkdir -p ${CMAKE_BINARY_DIR}/iso/boot/grub &&
  cp ${KERNEL_BINARY_DIR}/kernel ${CMAKE_BINARY_DIR}/iso/boot &&
  cp ${KERNEL_SOURCE}/boot/grub.cfg ${CMAKE_BINARY_DIR}/iso/boot/grub &&
  grub-mkrescue -o ${CMAKE_BINARY_DIR}/bubbl.iso ${CMAKE_BINARY_DIR}/iso

  DEPENDS kernel
)

add_custom_target(run
  ${QEMU_SYSTEM_CMD}
  ${QEMU_ARGUMENTS}
  -serial stdio
  ${IGNORE_EXIT}

  DEPENDS iso
  USES_TERMINAL
)

add_custom_target(run-headless
  ${QEMU_SYSTEM_CMD}
  ${QEMU_ARGUMENTS}
  -nographic
  ${IGNORE_EXIT}

  DEPENDS iso
  USES_TERMINAL
)

add_custom_target(run-bochs
  ${BOCHS_CMD}
  -f ${CMAKE_SOURCE_DIR}/.bochsrc

  DEPENDS iso
  USES_TERMINAL
)

add_custom_target(run-gdb
  ${QEMU_SYSTEM_CMD}
  ${QEMU_ARGUMENTS}
  -s
  -S

  ${IGNORE_EXIT}

  DEPENDS iso
  USES_TERMINAL
)

add_custom_target(clean-custom
  rm -rf
  ${CMAKE_BINARY_DIR}/iso
  ${CMAKE_BINARY_DIR}/bubbl.iso

  DEPENDS clean
)
