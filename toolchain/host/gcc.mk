VERSION := 14.2.0
URL := https://ftp.gnu.org/gnu/gcc/gcc-$(VERSION)/gcc-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/gcc-$(VERSION)
FILE := $(DIR)/gcc-$(VERSION).tar.gz
SRC := $(DIR)/gcc-$(VERSION)
OUT := $(OUT_DIR)/toolchain/host/gcc

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
	mkdir -p $(SRC)/build_host
	cd $(SRC)/build_host
	../configure --disable-nls --enable-languages=c,c++ --prefix=/

build: configure
	cd $(SRC)/build_host
	$(MAKE) -j$(PARALLEL_CORES)

install: build
	cd $(SRC)/build_host
	$(MAKE) install DESTDIR=$(OUT)

clean:
	rm -rf $(DIR) $(FILE)
