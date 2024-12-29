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
