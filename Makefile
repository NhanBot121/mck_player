# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Libraries and include directories
LIBS = -lSDL2 -lSDL2_mixer -ltag
INCLUDES = -I/usr/include/taglib

# Source files
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = mck_player

# Installation directory (system-wide path)
INSTALL_DIR = /usr/local/bin

# Build the executable
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(TARGET)

# Install the executable system-wide
install: $(TARGET)
	@echo "Installing $(TARGET) to $(INSTALL_DIR)..."
	@sudo cp $(TARGET) $(INSTALL_DIR)
	@sudo chmod +x $(INSTALL_DIR)/$(TARGET)
	@echo "$(TARGET) installed successfully!"

# Uninstall the executable
uninstall:
	@echo "Uninstalling $(TARGET) from $(INSTALL_DIR)..."
	@sudo rm -f $(INSTALL_DIR)/$(TARGET)
	@echo "$(TARGET) uninstalled successfully!"

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
