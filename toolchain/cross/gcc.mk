VERSION := 14.2.0
URL := https://ftp.gnu.org/gnu/gcc/gcc-$(VERSION)/gcc-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/toolchain/gcc-$(VERSION)
FILE := $(DIR)/gcc-$(VERSION).tar.gz
SRC := $(DIR)/gcc-$(VERSION)
OUT := $(OUT_DIR)/toolchain/cross

TARGET := i686-elf

all: install

.ONESHELL:

export PATH := $(OUT_DIR)/toolchain/host/gcc/bin:$(OUT_DIR)/toolchain/host/binutils/bin:$(OUT_DIR)/toolchain/cross/bin:$(PATH)

$(FILE):
	mkdir -p $(DIR)
	wget $(URL) -O $(FILE)

unpack: $(FILE)
	tar xf $(FILE) -C $(DIR)

configure: unpack
	cd $(SRC)
	./contrib/download_prerequisites
	mkdir -p $(SRC)/build_cross
	cd $(SRC)/build_cross
	../configure \
		--prefix=$(OUT) \
		--target=$(TARGET) \
		--disable-nls \
		--enable-languages=c,c++ \
		--without-headers \
		--disable-hosted-libstdcxx

build: configure
	cd $(SRC)/build_cross
	$(MAKE) -j$(PARALLEL_CORES) all-gcc
	$(MAKE) -j$(PARALLEL_CORES) all-target-libgcc
	$(MAKE) -j$(PARALLEL_CORES) all-target-libstdc++-v3

install: build
	cd $(SRC)/build_cross
	$(MAKE) install-gcc
	$(MAKE) install-target-libgcc
	$(MAKE) install-target-libstdc++-v3

clean:
	rm -rf $(DIR) $(FILE) $(OUT)
