# Generated by CMakePlugin
.PHONY: all clean $(MAKECMDGOALS)

CMAKE      := "D:\CMake\bin\cmake.exe"
BUILD_DIR  := build
SOURCE_DIR := ..
CMAKE_ARGS := -G "MinGW Makefiles"

# Building project(s)
$(or $(lastword $(MAKECMDGOALS)), all): $(BUILD_DIR)/Makefile
	$(MAKE) -C "$(BUILD_DIR)" $(MAKECMDGOALS)

# Building directory
$(BUILD_DIR):
	$(CMAKE) -E make_directory "$(BUILD_DIR)"

# Rule that detects if cmake is called
$(BUILD_DIR)/Makefile: .cmake_dirty | $(BUILD_DIR)
	cd "$(BUILD_DIR)" && $(CMAKE) $(CMAKE_ARGS) "$(SOURCE_DIR)"

# This rule / file allows force cmake run
.cmake_dirty:
	@echo '' > .cmake_dirty

