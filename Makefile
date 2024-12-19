# Documentation: https://www.gnu.org/software/make/manual/make.html
# Tutorial: https://makefiletutorial.com/#makefile-cookbook
# Automatic makefile dependencies: https://sparkyandsusi.wordpress.com/2016/11/26/automatic-makefile-dependencies/

# Only one recipe/target (only one rule has a recipe, other rules have only prerequisites)
# https://www.gnu.org/software/make/manual/make.html#Multiple-Rules

SRC_DIR := src
INCLUDE_DIR := include
BUILD_DIR := build

SRC_FILES := $(shell find $(SRC_DIR) -name "*.cpp")
HEADER_FILES := $(shell find $(INCLUDE_DIR) -name "*.hpp")
OBJECTS_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS_FILES := $(OBJECTS_FILES:.o=.d)

ASSETS_SRC_DIR := assets
ASSETS_DEST_DIR := $(BUILD_DIR)/assets
ASSETS_SRC_FILES := $(shell find $(ASSETS_SRC_DIR) -mindepth 1)

##################################################
# Compiling & linking
##################################################

# adding a new member field to included class headers changes memory layout => segfault if main not rebuilt
# -MMD: generate dependencies (*.d files) automatically
CPPFLAGS := -I$(INCLUDE_DIR)
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -MMD
LDFLAGS := -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# separate compilation from linking to recompile only modified file
$(BUILD_DIR)/main: $(OBJECTS_FILES)
	$(CXX) $(LDFLAGS) -o$@ $^

# compile target & generate deps files each time
# for subsequent builds, adds prerequistes from *.d files to rule (& runs same recipe)
# separate translation units to compile them in parallel (faster)
# Order-only prerequisites (right of pipe) won't run rule if dependency timestamp updated (i.e. run only once)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o$@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# more specific rules loaded from dependency files (for subsequent builds)
-include $(DEPS_FILES)

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
