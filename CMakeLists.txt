set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

cmake_minimum_required(VERSION 3.21)
project(CMOS)

add_subdirectory(kernel)

get_target_property(KERNEL_BINARY_DIR kernel BINARY_DIR)
get_target_property(KERNEL_SOURCE kernel SOURCE_DIR)

set(QEMU_SYSTEM_CMD qemu-system-i386)

set(QEMU_ARGUMENTS
    -kernel ${KERNEL_BINARY_DIR}/kernel
)

add_custom_target(iso
    mkdir -p ${CMAKE_BINARY_DIR}/iso/boot/grub &&
    cp ${KERNEL_BINARY_DIR}/kernel ${CMAKE_BINARY_DIR}/iso/boot &&
    cp ${KERNEL_SOURCE}/boot/grub.cfg ${CMAKE_BINARY_DIR}/iso/boot/grub &&
    grub-mkrescue -o ${CMAKE_BINARY_DIR}/cmos.iso ${CMAKE_BINARY_DIR}/iso

    DEPENDS kernel
)

add_custom_target(run
    ${QEMU_SYSTEM_CMD}
    ${QEMU_ARGUMENTS}

    DEPENDS kernel
)

add_custom_target(run-gdb
    ${QEMU_SYSTEM_CMD}
    ${QEMU_ARGUMENTS}
    -s
    -S

    DEPENDS kernel
)

add_custom_target(run-iso
    ${QEMU_SYSTEM_CMD} -cdrom ${CMAKE_BINARY_DIR}/cmos.iso

    DEPENDS iso
)
