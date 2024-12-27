VERSION := 14.2.0
URL := https://ftp.gnu.org/gnu/gcc/gcc-$(VERSION)/gcc-$(VERSION).tar.gz

DIR := $(BUILD_DIR)/gcc-$(VERSION)
FILE := $(DIR)/gcc-$(VERSION).tar.gz
SRC := $(DIR)/gcc-$(VERSION)
OUT := $(OUT_DIR)/gcc

all: install

.ONESHELL:

export PATH := $(OUT_DIR)/binutils/bin:$(PATH)

$(FILE):
	mkdir -p $(DIR)
	wget $(URL) -O $(FILE)

unpack: $(FILE)
	tar xf $(FILE) -C $(DIR)

configure: unpack
	cd $(SRC)
	./contrib/download_prerequisites
	mkdir -p $(SRC)/build
	cd $(SRC)/build
	../configure --disable-nls --enable-languages=c,c++

build: configure
	cd $(SRC)/build
	$(MAKE) -j$(PARALLEL_CORES)

install: build
	cd $(SRC)/build
	$(MAKE) install DESTDIR=$(OUT)

clean:
	rm -rf $(DIR) $(FILE)
