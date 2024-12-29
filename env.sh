#!/usr/bin/env bash

dir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PATH="$dir/toolchain/cross/bin:$dir/toolchain/host/bin:$PATH"

alias r="cd $dir"
alias b="mkdir -p $dir/build && cd $dir/build"

alias cmake="cmake -G Ninja"
alias n="ninja"
alias nr="ninja && ninja run"
