
include config.mk

.PHONY: all clean

.PHONY: lib shared example

all: lib example

lib:
	$(MAKE) -C $(LIB_MODULE_DIR)

shared:
	$(MAKE) -C $(LIB_MODULE_DIR) BUILD_SHARED=true

example: lib
	$(MAKE) -C $(EXAMPLE_MODULE_DIR)


clean:
	$(MAKE) -C $(LIB_MODULE_DIR) clean
	$(MAKE) -C $(EXAMPLE_MODULE_DIR) clean
	rm -rf $(ARTIFACT_ROOT_BASE)

