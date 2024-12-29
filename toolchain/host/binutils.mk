VERSION := 2.42
URL := https://ftp.gnu.org/gnu/binutils/binutils-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/toolchain/binutils-$(VERSION)
FILE := $(DIR)/binutils-$(VERSION).tar.gz
SRC := $(DIR)/binutils-$(VERSION)
OUT := $(OUT_DIR)/toolchain/host
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
