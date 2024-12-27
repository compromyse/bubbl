ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

PARALLEL_CORES := 16
BUILD_DIR := $(ROOT_DIR)/build
OUT_DIR := $(ROOT_DIR)/out

TOOLCHAIN_TARGETS := \
	toolchain/cc.mk
