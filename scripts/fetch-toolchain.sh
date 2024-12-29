#!/usr/bin/env bash

set -xe

rm -rf toolchain.tar.gz toolchain/host toolchain/cross
aria2c https://github.com/compromyse/cmos/releases/latest/download/toolchain.tar.gz

tar xvf toolchain.tar.gz
rm toolchain.tar.gz
