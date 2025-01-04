#
# CMOS
# Copyright (C) 2024-2025  Raghuram Subramani <raghus2247@gmail.com>
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

export ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
export BUILD_DIR := $(ROOT_DIR)/build
export OUT_DIR := $(ROOT_DIR)
export PARALLEL_CORES := 16

all:
	# $(MAKE) -f host_binutils.mk
	# $(MAKE) -f host_gcc.mk
	$(MAKE) -f cross_binutils.mk
	$(MAKE) -f cross_gcc.mk
