TARGET = aesf
CXX = g++

SOURCE_DIR = src
OBJECT_DIR = build

SOURCES = $(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SOURCES))

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

$(OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) -c -o $@ $^

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TARGET)