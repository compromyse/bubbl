set(QEMU_SYSTEM_CMD qemu-system-i386)

set(QEMU_ARGUMENTS
  -cdrom ${CMAKE_BINARY_DIR}/bubbl.iso
  -device isa-debug-exit,iobase=0xf4,iosize=0x04
  -m 32M
  -smp 1
)

set(IGNORE_EXIT || true)

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

add_custom_target(run-gdb
  ${QEMU_SYSTEM_CMD}
  ${QEMU_ARGUMENTS}
  -s
  -S

  ${IGNORE_EXIT}

  DEPENDS iso
  USES_TERMINAL
)

add_custom_target(run-debug
  ${QEMU_SYSTEM_CMD}
  ${QEMU_ARGUMENTS}
  -serial stdio
  -no-reboot
  -no-shutdown
  -d int
  -M smm=off

  ${IGNORE_EXIT}

  DEPENDS iso
  USES_TERMINAL
)
