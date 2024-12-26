VERSION := 2.42
BUILD_DIR := $(BUILD_DIR)/hostbinutils
FILE := $(BUILD_DIR)/binutils-$(VERSION).tar.gz
DIR := $(BUILD_DIR)/binutils-$(VERSION)
URL := https://ftp.gnu.org/gnu/binutils/binutils-$(VERSION).tar.gz

all: install

$(FILE):
	mkdir -p $(BUILD_DIR)
	wget $(URL) -O $(FILE)

unpacked: $(FILE)
	tar xf $(FILE) -C $(BUILD_DIR)

configured: unpacked
	mkdir -p $(DIR)/build
	cd $(DIR)/build && ../configure --prefix=$(BUILD_DIR) --disable-nls

built: configured
	$(MAKE) -C $(DIR)/build -j$(PARALLEL_CORES)

install: built
	$(MAKE) -C $(DIR)/build install

clean:
	rm -rf $(DIR) $(FILE) unpacked configured built installed
