cmake_minimum_required(VERSION 3.21)
project(kernel C ASM)

set(KERNEL_SRC
    boot/init/boot.s

    kernel/kernel.c
    kernel/stack_smashing_protector/stack_smashing_protector.c

    drivers/vga_text_buffer/vga_text_buffer.c

    libk/strlen.c
    libk/printk.c
)

add_executable(kernel ${KERNEL_SRC})
target_include_directories(kernel PRIVATE include)

set(C_COMPILE_OPTIONS
    -ffreestanding

    -fstack-protector
    -fstack-protector-all

    -O2

    -Wall
    -Wextra
    -pedantic
)

target_compile_options(kernel PRIVATE 
    $<$<COMPILE_LANGUAGE:C>: ${C_COMPILE_OPTIONS}>
)

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/boot/link.ld")
set(LINKER_FLAGS
    -T ${LINKER_SCRIPT}
    -nostdlib
)

execute_process(COMMAND ${CMAKE_C_COMPILER}
    -print-file-name=crtbegin.o
    OUTPUT_VARIABLE CRTBEGIN_O
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(COMMAND ${CMAKE_C_COMPILER}
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

set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <CMAKE_C_LINK_FLAGS> <FLAGS> <LINK_FLAGS> \
    ${CRTI_O} \
    ${CRTBEGIN_O} \
    <OBJECTS> \
    ${CRTEND_O} \
    ${CRTN_O} \
    -o <TARGET> <LINK_LIBRARIES>"
)

target_link_options(kernel PRIVATE ${LINKER_FLAGS})
