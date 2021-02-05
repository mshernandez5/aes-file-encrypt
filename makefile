# Target Executable, Output File
TARGET = aesf

# Compiler Options
CXX = g++
CXXFLAGS = -g -Wall

# Source & Build Directories
SOURCE_DIR = src
OBJECT_DIR = build

# Get List Of Source Files & Corresponding Object Files
SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SOURCES))

# Build Executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile Dependencies (Individual Source Files)
$(OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp | $(OBJECT_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $^

# Create Object Directory If It Doesn't Exist
$(OBJECT_DIR):
	mkdir $(OBJECT_DIR)

# Fake Targets, Point To Other Targets/Actions
# Rather Than Actual File Targets:

# Default Target
.PHONY: all
all: $(TARGET)

# Clean Files Before Compiling Executable
.PHONY: remake
remake: clean all

# Remove Executable & Compiled Objects
.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TARGET)

# Compile Executable With Optimizations, Removes Debugging Information
# Cleans Files To Ensure Unoptimized Files Are Not Included
.PHONY: release
release: CXXFLAGS = -O2 -s -DNDEBUG
release: clean $(TARGET)