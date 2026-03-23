# Shared build rules for module-level makefiles.
# Each module makefile should define BIN before including this file.

.PHONY: all clean

ifeq ($(DEBUG),true)
# Enable debug symbols for GNU toolchains.
CC = gcc -g
VERSION = debug
else
CC = gcc
VERSION = release
endif

# Collect all C source files in the current directory.
SRCS = $(wildcard *.c)

OBJS = $(SRCS:.c=.o)

# Convert source filenames to dependency filenames.
DEPS = $(SRCS:.c=.d)

# Place the final output under BUILD_ROOT.
BIN := $(addprefix $(BUILD_ROOT)/,$(BIN))

# Central directories for object and dependency outputs.
LINK_OBJ_DIR = $(BUILD_ROOT)/app/link_obj
DEP_DIR = $(BUILD_ROOT)/app/dep

# Create output directories if needed.
$(shell mkdir -p $(LINK_OBJ_DIR))
$(shell mkdir -p $(DEP_DIR))

OBJS := $(addprefix $(LINK_OBJ_DIR)/,$(OBJS))
DEPS := $(addprefix $(DEP_DIR)/,$(DEPS))

# Gather objects already generated in the shared object directory.
LINK_OBJ = $(wildcard $(LINK_OBJ_DIR)/*.o)
# Also include current target objects that may not exist yet when wildcard runs.
LINK_OBJ += $(OBJS)

all: $(DEPS) $(OBJS) $(BIN)

ifneq ("$(wildcard $(DEPS))","")
include $(DEPS)
endif

$(BIN): $(LINK_OBJ)
	@echo "------------------------build $(VERSION) mode--------------------------------!!!"

ifeq ($(BUILD_SO), true)
	# Build shared library output.
	$(CC) -fPIC -shared -o $@.so $^
else
	# Build executable output.
	$(CC) -o $@ $^
endif

$(LINK_OBJ_DIR)/%.o: %.c
	$(CC) -I$(INCLUDE_PATH) -o $@ -c $(filter %.c,$^)

$(DEP_DIR)/%.d: %.c
	echo -n $(LINK_OBJ_DIR)/ > $@
	# Append compiler-generated dependency rules.
	gcc -I$(INCLUDE_PATH) -MM $^ >> $@

clean:
	rm -f $(BIN) $(OBJS) $(DEPS) *.gch
