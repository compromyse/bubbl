#!/usr/bin/env bash

dir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PATH="$dir/toolchain/cross/bin:$dir/toolchain/host/bin:$PATH"

alias r="cd $dir"
alias b="mkdir -p $dir/build && cd $dir/build"

CMAKE_FLAGS="-G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=true"
alias cm="cmake $CMAKE_FLAGS"
alias n="ninja"
alias nc="ninja clean"
alias nr="ninja run"
alias ng="ninja run-gdb"
alias ni="ninja iso"
alias nri="ninja run-iso"
