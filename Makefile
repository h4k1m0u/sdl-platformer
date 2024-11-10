# Documentation: https://www.gnu.org/software/make/manual/make.html
# Tutorial: https://makefiletutorial.com/#makefile-cookbook

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

SRC_FILES := $(shell find $(SRC_DIR) -name "*.cpp")
HEADER_FILES := $(shell find $(INCLUDE_DIR) -name "*.hpp")
OBJECTS_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

ASSETS_SRC_DIR := assets
ASSETS_DEST_DIR := $(BUILD_DIR)/assets
ASSETS_SRC_FILES := $(shell find $(ASSETS_SRC_DIR) -mindepth 1)

##################################################
# Compiling & linking
##################################################

CPPFLAGS := -I$(INCLUDE_DIR)
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror
LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# separate compilation from linking to recompile only modified file
$(BUILD_DIR)/main: $(OBJECTS_FILES)
	$(CXX) $(LDFLAGS) -o$@ $^

# separate translation units to compile them in parallel (faster)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDE_DIR)/%.hpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o$@ $<

# most specific rules used for main.cpp, not the above one (no header file)
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o$@ $<

##################################################
# Assets
##################################################

$(ASSETS_DEST_DIR): $(ASSETS_SRC_FILES)
	mkdir -p $@
	cp $^ $@

##################################################
# Utils
##################################################

.PHONY: clean print
clean:
	rm -rf $(BUILD_DIR)

print:
	@echo "objects: $(OBJECTS_FILES)"
	@echo "src: $(SRC_FILES)"
	@echo "assets: $(ASSETS_SRC_FILES)"
