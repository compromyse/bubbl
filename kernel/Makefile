include $(ROOT_DIR)/make/utils.mk
include $(ROOT_DIR)/toolchain/constants.mk

$(call add-to-path,$(HOST_TOOLCHAIN))
KERNEL_ROOT := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
BUILD = $(BUILD_DIR)/kernel/build

src :=

include $(call all-subdir-makefiles)

all:
	echo $(src)
