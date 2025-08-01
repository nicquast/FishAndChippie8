TARGET_DIR := ./bin
TARGET_EXEC := fnc8
BUILD_DIR := ./build
SRC_DIR := ./src

CC := gcc
CFLAGS := -std=c99 -Wall -Werror

# Find the C source files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
H_FILES := $(shell find $(SRC_DIR) -name '*.h')

# Get a list of object files from the C files found
OBJS := $(SRC_FILES:%=$(BUILD_DIR)/%.o)

# Get all subfolders/modules in src directory
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $^ $(CFLAGS) $(INC_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(H_FILES) #figure out a way to only add needed ones
	$(CC) -c -o $@ $< $(CFLAGS) $(INC_FLAGS)

.PHONY: clean

clean:
	rm -f $(OBJS)
