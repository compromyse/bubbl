#!/usr/bin/env bash

#
# CMOS
# Copyright (C) 2024-2025 Raghuram Subramani <raghus2247@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

dir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PATH="$dir/toolchain/cross/bin:$dir/toolchain/host/bin:$PATH"

alias r="cd $dir"
alias b="mkdir -p $dir/build && cd $dir/build"

CMAKE_FLAGS="-G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=true"
alias cm="cmake $CMAKE_FLAGS"

alias m="ninja"
alias mc="ninja clean-custom"
alias mi="ninja iso"
alias mr="ninja run"
alias mg="ninja run-gdb"
