include config.mk

export ROOT_DIR
export BUILD_DIR
export OUT_DIR
export PARALLEL_CORES

# TARGETS
all: $(TARGETS)

.PHONY: $(TARGETS)
$(TARGETS):
	$(MAKE) -f $@

# TOOLCHAINS
toolchain: $(TOOLCHAIN_TARGETS)

toolchain-clean:
	for target in $(TOOLCHAIN_TARGETS); do \
		$(MAKE) -f $$target clean; \
	done

.PHONY: $(TOOLCHAIN_TARGETS)
$(TOOLCHAIN_TARGETS):
	$(MAKE) -f $@

clean:
	rm -rf $(BUILD_DIR)
