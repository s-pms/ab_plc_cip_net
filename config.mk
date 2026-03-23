# Root directory of this repository.
export BUILD_ROOT = $(shell pwd)

# Include path used by common.mk compile rules.
export INCLUDE_PATH = $(BUILD_ROOT)/ab_plc_cip_net

# Subdirectories to build from top-level makefile.
BUILD_DIR = $(BUILD_ROOT)/ab_plc_cip_net/

# true: debug build with symbols; false: release build.
export DEBUG = true

# true: build shared object; false: build executable.
export BUILD_SO = false 

