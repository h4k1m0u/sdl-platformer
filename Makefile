# Documentation: https://www.gnu.org/software/make/manual/make.html
# Tutorial: https://makefiletutorial.com/#makefile-cookbook

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

SRC_FILES := $(shell find $(SRC_DIR) -name "*.cpp")
HEADER_FILES := $(shell find $(INCLUDE_DIR) -name "*.hpp")
OBJECTS := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

CPPFLAGS := -I$(INCLUDE_DIR)
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror
LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf

# separate compilation from linking to recompile only modified file
$(BUILD_DIR)/main: $(OBJECTS)
	$(CXX) $(LDFLAGS) -o$@ $(OBJECTS)

# separate translation units to compile them in parallel (faster)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o$@ $<

# most specific rules used for main.cpp, not the above one (no header file)
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o$@ $<

.PHONY: clean print
clean:
	rm -rf $(BUILD_DIR)

print:
	@echo "objects: $(OBJECTS)"
	@echo "src: $(SRC_FILES)"
