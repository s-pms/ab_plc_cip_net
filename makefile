
include config.mk

.PHONY: all clean

.PHONY: lib shared example install package publish install-shared package-shared publish-shared

all: lib example

lib:
	$(MAKE) -C $(LIB_MODULE_DIR)

shared:
	$(MAKE) -C $(LIB_MODULE_DIR) BUILD_SHARED=true

example: lib
	$(MAKE) -C $(EXAMPLE_MODULE_DIR)

install: all
	$(MKDIR_P) $(INSTALL_INCLUDE_DIR) $(INSTALL_LIB_DIR) $(INSTALL_DOC_DIR)
	$(CP) $(PUBLIC_HEADER_PATHS) $(INSTALL_INCLUDE_DIR)/
	$(CP) $(STATIC_LIBRARY) $(INSTALL_LIB_DIR)/
	@if [ -f "$(ARTIFACT_ROOT)/bin/$(EXAMPLE_NAME)" ]; then \
		$(MKDIR_P) $(INSTALL_BIN_DIR); \
		$(CP) "$(ARTIFACT_ROOT)/bin/$(EXAMPLE_NAME)" $(INSTALL_BIN_DIR)/; \
	fi
	$(CP) $(PACKAGE_DOCS) $(INSTALL_DOC_DIR)/

package: install

publish: install

install-shared: shared
	$(MKDIR_P) $(INSTALL_INCLUDE_DIR) $(INSTALL_LIB_DIR) $(INSTALL_DOC_DIR)
	$(CP) $(PUBLIC_HEADER_PATHS) $(INSTALL_INCLUDE_DIR)/
	$(CP) $(SHARED_LIBRARY) $(INSTALL_LIB_DIR)/
	$(CP) $(PACKAGE_DOCS) $(INSTALL_DOC_DIR)/

package-shared: install-shared

publish-shared: install-shared


clean:
	$(MAKE) -C $(LIB_MODULE_DIR) clean
	$(MAKE) -C $(EXAMPLE_MODULE_DIR) clean
	$(RM_RF) $(ARTIFACT_ROOT_BASE) $(DIST_ROOT_BASE)

