#!/usr/bin/env bash

set -ex

TARGET=i686-elf
BINUTILSVERSION=2.43
GCCVERSION=14.2.0

export MAKE=make
export TAR=tar

export CFLAGS="-O2 -pipe"

unset CC
unset CXX

OUT_PATH="$(pwd)/../../out"
PREFIX="$OUT_PATH/toolchain/cross"
HOST_GCC_PATH="$OUT_PATH/toolchain/host/gcc/bin"
HOST_BINUTILS_PATH="$OUT_PATH/toolchain/host/binutils/bin"

mkdir -p build && cd build

export MAKEFLAGS="-j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || psrinfo -tc 2>/dev/null || echo 1)"

export PATH="$PREFIX/bin:$HOST_GCC_PATH:$HOST_BINUTILS_PATH:$PATH"

if [ ! -f binutils-$BINUTILSVERSION.tar.gz ]; then
    curl -o binutils-$BINUTILSVERSION.tar.gz https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILSVERSION.tar.gz
fi
if [ ! -f gcc-$GCCVERSION.tar.gz ]; then
    curl -o gcc-$GCCVERSION.tar.gz https://ftp.gnu.org/gnu/gcc/gcc-$GCCVERSION/gcc-$GCCVERSION.tar.gz
fi

rm -rf build
mkdir build
cd build

$TAR -zxf ../binutils-$BINUTILSVERSION.tar.gz
$TAR -zxf ../gcc-$GCCVERSION.tar.gz

mkdir build-binutils
cd build-binutils
../binutils-$BINUTILSVERSION/configure CFLAGS="$CFLAGS" CXXFLAGS="$CFLAGS" --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
$MAKE
$MAKE install
cd ..

cd gcc-$GCCVERSION
./contrib/download_prerequisites
cd ..
mkdir build-gcc
cd build-gcc
../gcc-$GCCVERSION/configure CFLAGS="$CFLAGS" CXXFLAGS="$CFLAGS" --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
$MAKE all-gcc
$MAKE all-target-libgcc
$MAKE install-gcc
$MAKE install-target-libgcc
cd ..
