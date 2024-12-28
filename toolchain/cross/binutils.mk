VERSION := 2.42
URL := https://ftp.gnu.org/gnu/binutils/binutils-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/binutils-$(VERSION)
FILE := $(DIR)/binutils-$(VERSION).tar.gz
SRC := $(DIR)/binutils-$(VERSION)
OUT := $(OUT_DIR)/toolchain/cross

TARGET := i686-elf

all: install

.ONESHELL:

export PATH := $(OUT_DIR)/toolchain/host/gcc/bin:$(OUT_DIR)/toolchain/host/binutils/bin:$(PATH)

$(FILE):
	mkdir -p $(DIR)
	wget $(URL) -O $(FILE)

unpack: $(FILE)
	tar xf $(FILE) -C $(DIR)

configure: unpack
	mkdir -p $(SRC)/build_cross
	cd $(SRC)/build_cross
	../configure \
		--prefix=$(OUT) \
		--target=$(TARGET) \
		--disable-nls \
		--with-sysroot

build: configure
	cd $(SRC)/build_cross
	$(MAKE) -j$(PARALLEL_CORES)

install: build
	cd $(SRC)/build_cross
	$(MAKE) install

clean:
	rm -rf $(DIR) $(FILE) $(OUT)
