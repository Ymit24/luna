# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I./src

# Source and build directories
SRC_DIR = src
BUILD_DIR = build

# Source files and target executable
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET = $(BUILD_DIR)/my_program

# Default rule
all: $(TARGET)

# Link objects to create the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	@echo "Built $(TARGET)"

# Compile source files into objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned build artifacts."

# Phony targets
.PHONY: all clean
