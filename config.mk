# Repository root and module layout.
BUILD_ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
LIB_MODULE_DIR := $(BUILD_ROOT)/ab_plc_cip_net
EXAMPLE_MODULE_DIR := $(BUILD_ROOT)/examples

# Public headers live with the library sources.
INCLUDE_PATH := $(LIB_MODULE_DIR)

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

export BUILD_ROOT
export LIB_MODULE_DIR
export EXAMPLE_MODULE_DIR
export INCLUDE_PATH
export DEBUG
export BUILD_SHARED
export LIB_NAME
export EXAMPLE_NAME
export BUILD_FLAVOR
export ARTIFACT_ROOT_BASE
export ARTIFACT_ROOT
export STATIC_LIBRARY
export SHARED_LIBRARY

