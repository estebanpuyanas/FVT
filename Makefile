.PHONY: help all clean run nuke update-glob-bin

help:
	@echo "Available commands:"
	@echo "make clean: Remove build files and binaries from their respective directories."
	@echo "make update-glob-bin: Update the global binary in /usr/local/bin with the latest build; Helpful for experimental/nightly builds."
	@echo "make nuke: Deletes any test repos created inside the project repository (i.e anything containing a .fvt directory)."
	@echo "make run: Run the program."
	@echo "make all: Build the project."
	@echo "make: Build the project"

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Output binary name
TARGET = $(BIN_DIR)/fvt

# Source and object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lssl -lcrypto -lz

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

run: all
	./$(TARGET)

nuke:
	find . -type d -name ".fvt" -prune -execdir bash -c 'cd .. && rm -rf "$$(basename "$$PWD")"' \;

update-glob-bin: all
	@if [ ! -f $(TARGET) ]; then \
    	echo "Error: $(TARGET) not found. Please build the project with 'make' first."; \
    	exit 1; \
	fi
	@sudo cp $(TARGET) /usr/local/bin/fvt
	@sudo chmod +x /usr/local/bin/fvt
