include ../config.mk

TARGET = $(LIB_NAME)
SRCS = ab_cip.c ab_cip_helper.c socket.c utill.c
INCLUDE_DIRS = $(INCLUDE_PATH)

ifeq ($(BUILD_SHARED),true)
TARGET_TYPE = shared
else
TARGET_TYPE = static
endif

include ../common.mk




