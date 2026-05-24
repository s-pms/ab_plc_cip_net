# Shared build rules for module-level makefiles.
# Required variables before including this file:
#   TARGET       logical target name without prefix/suffix
#   TARGET_TYPE  static | shared | executable
#   SRCS         one or more .c source files

ifndef TARGET
$(error TARGET is required before including common.mk)
endif

ifndef TARGET_TYPE
$(error TARGET_TYPE is required before including common.mk)
endif

ifndef SRCS
$(error SRCS is required before including common.mk)
endif

.PHONY: all clean

CC ?= gcc
AR ?= ar
RM ?= rm -f
RM_RF ?= rm -rf
MKDIR_P ?= mkdir -p

ifeq ($(DEBUG),true)
CFLAGS_COMMON ?= -g -O0
else
CFLAGS_COMMON ?= -O2
endif

OBJ_DIR := $(ARTIFACT_ROOT)/obj/$(TARGET)
DEP_DIR := $(ARTIFACT_ROOT)/dep/$(TARGET)

ifeq ($(TARGET_TYPE),static)
TARGET_DIR ?= $(ARTIFACT_ROOT)/lib
TARGET_FILE := lib$(TARGET).a
LINK_COMMAND = $(AR) rcs $@ $(OBJS)
else ifeq ($(TARGET_TYPE),shared)
TARGET_DIR ?= $(ARTIFACT_ROOT)/lib
TARGET_FILE := lib$(TARGET).so
TARGET_CFLAGS += -fPIC
LINK_COMMAND = $(CC) -shared -o $@ $(OBJS) $(TARGET_LINK_INPUTS) $(TARGET_LDLIBS)
else ifeq ($(TARGET_TYPE),executable)
TARGET_DIR ?= $(ARTIFACT_ROOT)/bin
TARGET_FILE := $(TARGET)
LINK_COMMAND = $(CC) -o $@ $(OBJS) $(TARGET_LINK_INPUTS) $(TARGET_LDLIBS)
else
$(error Unsupported TARGET_TYPE '$(TARGET_TYPE)')
endif

TARGET_OUTPUT := $(TARGET_DIR)/$(TARGET_FILE)
SOURCE_DIRS := $(sort $(dir $(SRCS)))
VPATH := $(SOURCE_DIRS)
OBJS := $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.c=.o)))
DEPS := $(addprefix $(DEP_DIR)/,$(notdir $(SRCS:.c=.d)))

ifneq ($(strip $(INCLUDE_DIRS)),)
INCLUDE_FLAGS := $(addprefix -I,$(INCLUDE_DIRS))
endif

CFLAGS := $(CFLAGS_COMMON) $(TARGET_CFLAGS) $(INCLUDE_FLAGS)

all: $(TARGET_OUTPUT)

$(TARGET_DIR) $(OBJ_DIR) $(DEP_DIR):
	@$(MKDIR_P) $@

$(TARGET_OUTPUT): $(OBJS) $(TARGET_PREREQS) | $(TARGET_DIR)
	$(LINK_COMMAND)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(DEP_DIR)
	$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@

-include $(DEPS)

clean:
	$(RM_RF) $(OBJ_DIR) $(DEP_DIR)
	$(RM) $(TARGET_OUTPUT)
