CXX := clang++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -Iinclude -g

SRC_DIR := src
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
HDRS := $(wildcard include/*.hpp)
TIDY_CHECKS := bugprone-*,clang-analyzer-*,modernize-*,performance-*,readability-*

TARGET := $(BUILD_DIR)/binfo

.PHONY: all clean run tidy format

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: build
	./$(TARGET)

tidy:
	clang-tidy $(SRCS) -p $(BUILD_DIR) --checks='$(TIDY_CHECKS)'

format:
	clang-format -i $(SRCS) $(HDRS)
