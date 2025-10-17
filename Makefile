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

# Build the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lssl -lcrypto -lz

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean build files
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run the program
run: all
	./$(TARGET)

# Deletes any test repos created inside the project (i.e anything containing a .fvt directory)
nuke:
	find . -type d -name ".fvt" -prune -execdir bash -c 'cd .. && rm -rf "$$(basename "$$PWD")"' \;

# Update the global binary in /usr/local/bin 
update-glob-bin: all
	@if [ ! -f $(TARGET) ]; then \
    	echo "Error: $(TARGET) not found. Please build the project with 'make' first."; \
    	exit 1; \
	fi
	@sudo cp $(TARGET) /usr/local/bin/fvt
	@sudo chmod +x /usr/local/bin/fvt

.PHONY: all clean run nuke update-glob-bin