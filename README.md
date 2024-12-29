# cmos

## Building

### Dependencies:

- CMake
- Ninja
- Aria2 (for fetch-toolchain.sh)

```sh
./scripts/fetch-toolchain.sh

source env.sh
mkdir build && cd build
cmake ..
ninja build
```

## Running

```sh
ninja run
```

## Authors

- [@compromyse](https://www.github.com/compromyse)
