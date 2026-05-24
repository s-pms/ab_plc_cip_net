# Repository root and module layout.
BUILD_ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
LIB_MODULE_DIR := $(BUILD_ROOT)/ab_plc_cip_net
EXAMPLE_MODULE_DIR := $(BUILD_ROOT)/examples

MKDIR_P ?= mkdir -p
CP ?= cp -f
RM_RF ?= rm -rf

# Public headers live with the library sources.
INCLUDE_PATH := $(LIB_MODULE_DIR)
PUBLIC_HEADERS := ab_cip.h typedef.h
PUBLIC_HEADER_PATHS := $(addprefix $(LIB_MODULE_DIR)/,$(PUBLIC_HEADERS))
PACKAGE_DOCS := $(BUILD_ROOT)/LICENSE $(BUILD_ROOT)/README.md $(BUILD_ROOT)/README_EN.md

# Build mode switches. Override on the command line, for example:
#   make DEBUG=false
#   make shared BUILD_SHARED=true
DEBUG ?= true
BUILD_SHARED ?= false

# Public target names.
LIB_NAME := ab_plc_cip_net
EXAMPLE_NAME := ab_cip_test

ifeq ($(DEBUG),true)
BUILD_FLAVOR := debug
else
BUILD_FLAVOR := release
endif

# All generated artifacts are stored under artifacts/.
ARTIFACT_ROOT_BASE := $(BUILD_ROOT)/artifacts
ARTIFACT_ROOT := $(ARTIFACT_ROOT_BASE)/$(BUILD_FLAVOR)
STATIC_LIBRARY := $(ARTIFACT_ROOT)/lib/lib$(LIB_NAME).a
SHARED_LIBRARY := $(ARTIFACT_ROOT)/lib/lib$(LIB_NAME).so

# Default install/export layout.
DIST_ROOT_BASE := $(BUILD_ROOT)/dist
DIST_ROOT := $(DIST_ROOT_BASE)/$(BUILD_FLAVOR)
DESTDIR ?=
PREFIX ?= $(DIST_ROOT)
INSTALL_ROOT := $(DESTDIR)$(PREFIX)
INSTALL_INCLUDE_DIR := $(INSTALL_ROOT)/include
INSTALL_LIB_DIR := $(INSTALL_ROOT)/lib
INSTALL_BIN_DIR := $(INSTALL_ROOT)/bin
INSTALL_DOC_DIR := $(INSTALL_ROOT)/share/doc/$(LIB_NAME)

export BUILD_ROOT
export LIB_MODULE_DIR
export EXAMPLE_MODULE_DIR
export MKDIR_P
export CP
export RM_RF
export INCLUDE_PATH
export PUBLIC_HEADERS
export PUBLIC_HEADER_PATHS
export PACKAGE_DOCS
export DEBUG
export BUILD_SHARED
export LIB_NAME
export EXAMPLE_NAME
export BUILD_FLAVOR
export ARTIFACT_ROOT_BASE
export ARTIFACT_ROOT
export STATIC_LIBRARY
export SHARED_LIBRARY
export DIST_ROOT_BASE
export DIST_ROOT
export DESTDIR
export PREFIX
export INSTALL_ROOT
export INSTALL_INCLUDE_DIR
export INSTALL_LIB_DIR
export INSTALL_BIN_DIR
export INSTALL_DOC_DIR

