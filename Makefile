include config.mk

all: $(TARGETS)

.PHONY: $(TARGETS)
$(TARGETS):
	$(MAKE) -f $@ BUILD_DIR=$(BUILD_DIR) PARALLEL_CORES=$(PARALLEL_CORES)

clean:
	rm -rf $(BUILD_DIR)
