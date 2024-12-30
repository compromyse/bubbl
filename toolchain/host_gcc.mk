#
# CMOS
# Copyright (C) 2024 Raghuram Subramani <raghus2247@gmail.com>
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

VERSION := 14.2.0
URL := https://ftp.gnu.org/gnu/gcc/gcc-$(VERSION)/gcc-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/toolchain/gcc-$(VERSION)
FILE := $(DIR)/gcc-$(VERSION).tar.gz
SRC := $(DIR)/gcc-$(VERSION)
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
    cd $(SRC)
    ./contrib/download_prerequisites
    mkdir -p $(BUILD)
    cd $(BUILD)
    $(SRC)/configure \
	--prefix=$(OUT) \
	--disable-nls \
	--enable-languages=c,c++

build: configure
    cd $(BUILD)
    $(MAKE) -j$(PARALLEL_CORES)

install: build
    cd $(BUILD)
    $(MAKE) install

clean:
    rm -rf $(DIR) $(FILE) $(OUT)
