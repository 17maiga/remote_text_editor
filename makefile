# Variables
CC := gcc
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
TARGET := bin/editor
SRC_EXT := c
SOURCES := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OBJECTS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))
CFLAGS := -g -Wall -Wextra -lpthread
INC := -I inc

# Execution command
# Builds the executable if needed and runs it
run: $(TARGET)
	@echo "MAKEFILE :: Running..."
	@echo "MAKEFILE :: ./$(TARGET)"; ./$(TARGET)

# Main build process
# Creates the executable in the bin directory
build: $(TARGET)

# Executable file
$(TARGET): $(OBJECTS)
	@echo "MAKEFILE :: Linking..."
	@mkdir -p $(BIN_DIR)
	@echo "MAKEFILE :: $(CC) $^ $(CFLAGS) -o $(TARGET)"; $(CC) $^ $(CFLAGS) -o $(TARGET)

# Build files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@echo "MAKEFILE :: Building..."
	@mkdir -p $(BUILD_DIR)
	@echo "MAKEFILE :: $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Removes the object files and the executables
clean:
	@echo "MAKEFILE :: Cleaning..."
	@echo "MAKEFILE :: $(RM) -r $(BUILD_DIR) $(BIN_DIR)"; $(RM) -r $(BUILD_DIR) $(BIN_DIR)
