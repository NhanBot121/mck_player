# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++23 -Iinc

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files (compiled .o files)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable
TARGET = $(BIN_DIR)/program

# Default rule
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ_FILES)

# Rule to compile .cpp files into .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean run

run: all
	@./$(TARGET)
