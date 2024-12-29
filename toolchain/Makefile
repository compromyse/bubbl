export ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
export BUILD_DIR := $(ROOT_DIR)/build
export OUT_DIR := $(ROOT_DIR)
export PARALLEL_CORES := 16

all:
	# $(MAKE) -f host_binutils.mk
	# $(MAKE) -f host_gcc.mk
	$(MAKE) -f cross_binutils.mk
	$(MAKE) -f cross_gcc.mk
