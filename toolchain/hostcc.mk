VERSION := 14.1.0
BUILD_DIR := $(BUILD_DIR)/hostcc
FILE := $(BUILD_DIR)/gcc-$(VERSION).tar.gz
DIR := $(BUILD_DIR)/gcc-$(VERSION)
URL := https://ftp.gnu.org/gnu/gcc-$(VERSION)/gcc-$(VERSION).tar.gz

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
