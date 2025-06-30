# Compiler and project settings
CXX        = g++
CXXFLAGS   = -std=c++17 -Wall -Wextra -g -MMD -MP # -MMD -MP are for auto-dependency generation
LDFLAGS    =
TARGET_DIR = bin
OBJ_DIR    = obj
TARGET     = $(TARGET_DIR)/matcher

# Automatically find sources and define corresponding objects and dependencies
SOURCES    = $(wildcard src/*.cpp)
OBJECTS    = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES)) # Map sources to object files in OBJ_DIR
DEPS       = $(OBJECTS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_DIR)
	$(CXX) $(LDFLAGS) $^ -o $@

# Compile source files into object files
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@

# Include generated dependency files. The `-` suppresses errors if they don't exist yet.
-include $(DEPS)

clean:
	@rm -rf $(OBJ_DIR) $(TARGET_DIR)

run: all
	@./$(TARGET)