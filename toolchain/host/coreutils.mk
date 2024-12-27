VERSION := 9.5
URL := https://ftp.gnu.org/gnu/coreutils/coreutils-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/coreutils-$(VERSION)
FILE := $(DIR)/coreutils-$(VERSION).tar.gz
SRC := $(DIR)/coreutils-$(VERSION)
OUT := $(OUT_DIR)/toolchain/host/coreutils

all: install

.ONESHELL:

$(FILE):
	mkdir -p $(DIR)
	wget $(URL) -O $(FILE)

unpack: $(FILE)
	tar xf $(FILE) -C $(DIR)

configure: unpack
	mkdir -p $(SRC)/build
	cd $(SRC)/build
	../configure --disable-nls --prefix=/

build: configure
	cd $(SRC)/build
	$(MAKE) -j$(PARALLEL_CORES)

install: build
	cd $(SRC)/build
	$(MAKE) install DESTDIR=$(OUT)

clean:
	rm -rf $(DIR) $(FILE)
