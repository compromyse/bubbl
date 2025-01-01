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

VERSION := 2.42
URL := https://ftp.gnu.org/gnu/binutils/binutils-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/toolchain/binutils-$(VERSION)
FILE := $(DIR)/binutils-$(VERSION).tar.gz
SRC := $(DIR)/binutils-$(VERSION)
OUT := $(OUT_DIR)/host
BUILD := $(DIR)/build_host

all: install

.ONESHELL:

$(FILE):
	mkdir -p $(DIR)
	wget $(URL) -O $(FILE)

unpack: $(FILE)
	tar xf $(FILE) -C $(DIR)

configure: unpack
	mkdir -p $(BUILD)
	cd $(BUILD)
	$(SRC)/configure \
		--prefix=$(OUT) \
		--disable-nls

build: configure
	cd $(BUILD)
	$(MAKE) -j$(PARALLEL_CORES)

install: build
	cd $(BUILD)
	$(MAKE) install

clean:
	rm -rf $(DIR) $(FILE) $(OUT)
