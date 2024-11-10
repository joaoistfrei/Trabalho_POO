# Compiler and flags
CXX = g++
CXXFLAGS = -Iinclude -c -std=c++14 -Wall

# Source directories
SRC_DIRS = boardgame chess chess/pieces application

# Find all .cpp files in the source directories
SOURCES = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))

# Generate object files from .cpp sources
OBJECTS = $(patsubst %.cpp, bin/%.o, $(SOURCES))

# Target executable
TARGET = bin/my_program

# Default rule
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Compilation rule
bin/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean rule
clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf bin

.PHONY: all clean
