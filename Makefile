CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -Iinclude -g

SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
HDRS := $(wildcard include/*.hpp)
TIDY_CHECKS := bugprone-*,clang-analyzer-*,modernize-*,-modernize-use-trailing-return-type,performance-*,readability-*

TARGET := $(BUILD_DIR)/binfo

CMAKE_BUILD_DIR := build/cmake

.PHONY: all clean run tidy format test

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: build
	./$(TARGET)

tidy:
	clang-tidy $(SRCS) --checks='$(TIDY_CHECKS)' -- $(CXXFLAGS)

format:
	clang-format -i $(SRCS) $(HDRS)

test:
	cmake -B $(CMAKE_BUILD_DIR) -S . -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(CMAKE_BUILD_DIR)
	ctest --test-dir $(CMAKE_BUILD_DIR) --output-on-failure
