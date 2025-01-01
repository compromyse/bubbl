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

include utils.mk

VERSION := 14.2.0
URL := https://ftp.gnu.org/gnu/gcc/gcc-$(VERSION)/gcc-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/toolchain/gcc-$(VERSION)
FILE := $(DIR)/gcc-$(VERSION).tar.gz
SRC := $(DIR)/gcc-$(VERSION)
OUT := $(OUT_DIR)/cross
BUILD := $(DIR)/build_cross

TOOLCHAIN_TARGET := i686-elf

all: install

.ONESHELL:

$(call add-to-path,$(shell pwd)/cross/bin)
$(call add-to-path,$(shell pwd)/host/bin)

$(FILE):
	mkdir -p $(DIR)
	wget $(URL) -O $(FILE)

unpack: $(FILE)
	tar xf $(FILE) -C $(DIR)

configure: unpack
	cd $(SRC)
	./contrib/download_prerequisites
	mkdir -p $(BUILD)
	cd $(BUILD)
	$(SRC)/configure \
		--prefix=$(OUT) \
		--target=$(TOOLCHAIN_TARGET) \
		--disable-nls \
		--enable-languages=c,c++ \
		--without-headers \
		--disable-hosted-libstdcxx

build: configure
	cd $(BUILD)
	$(MAKE) -j$(PARALLEL_CORES) all-gcc
	$(MAKE) -j$(PARALLEL_CORES) all-target-libgcc
	$(MAKE) -j$(PARALLEL_CORES) all-target-libstdc++-v3

install: build
	cd $(BUILD)
	$(MAKE) install-gcc
	$(MAKE) install-target-libgcc
	$(MAKE) install-target-libstdc++-v3

clean:
	rm -rf $(DIR) $(FILE) $(OUT)
