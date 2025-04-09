# Bubbl

An Operating System for The x86 Architecture.

## Building

> Dependencies: nix with flakes enabled and (optionally) direnv

After entering the devshell (with direnv or `nix develop`),

```sh
mkdir build && cd build
cmake -G Ninja ..
ninja
```

## Running

```sh
ninja run # or run-headless or run-gdb
```

## Authors

- [@compromyse](https://www.github.com/compromyse)
