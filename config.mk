PARALLEL_CORES := 16
BUILD_DIR := $(ROOT_DIR)/build
OUT_DIR := $(ROOT_DIR)/out

TARGETS := \
	kernel

TOOLCHAIN_TARGETS := \
	toolchain/host/binutils.mk \
	toolchain/host/gcc.mk

TOOLCHAIN_TARGETS += \
	toolchain/cross/binutils.mk \
	toolchain/cross/gcc.mk
