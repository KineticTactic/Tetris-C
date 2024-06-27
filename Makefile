# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g -O0

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = ./dep/raylib/include  # Directory for header files
BUILD_DIR = build
LIB_DIR = ./dep/raylib/lib  # Directory for static libraries

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Include paths
INCLUDE = -I$(INCLUDE_DIR)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Libraries to link
LIBS = -L$(LIB_DIR) -lraylib -lgdi32 -lwinmm # Replace "libexample.a" with your library name

# Executable name
TARGET = $(BUILD_DIR)/main

# Rule to build the target
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Phony targets
.PHONY: clean
