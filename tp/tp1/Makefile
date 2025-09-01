# Makefile for Influencer Marketing Problem

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
TARGET = influencer_solver
SOURCES = main.cpp Instance.cpp Solution.cpp BruteForceSolver.cpp BacktrackingSolver.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = Instance.h Solution.h BruteForceSolver.h BacktrackingSolver.h

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Run with a test instance (assuming there's a test file)
test: $(TARGET)
	@echo "Usage: make test INSTANCE=<path_to_instance_file>"
	@echo "Example: make test INSTANCE=selected_instances/ninstance_305.txt"
ifdef INSTANCE
	./$(TARGET) $(INSTANCE)
endif

# Help target
help:
	@echo "Available targets:"
	@echo "  all     - Build the main executable (default)"
	@echo "  clean   - Remove build files"
	@echo "  debug   - Build with debug information"
	@echo "  test    - Run with test instance (use INSTANCE=<file>)"
	@echo "  help    - Show this help message"

.PHONY: all clean debug test help
