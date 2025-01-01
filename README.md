# CMOS

An Operating System for The x86 Architecture.

## Building

### Dependencies:

- CMake
- Ninja
- Aria2 (for fetch-toolchain.sh)
- GRUB (grub-mkrescue)
- xorriso

```sh
./scripts/fetch-toolchain.sh

source env.sh
mkdir build && cd build
cm ..
ninja build
```

## Running

### Dependencies:

- QEMU (qemu-system-i386)

```sh
ninja run
```

## Authors

- [@compromyse](https://www.github.com/compromyse)
