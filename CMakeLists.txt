cmake_minimum_required(VERSION 3.20)

project(
    binfo
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

include_directories(include)

# Shared library: everything except main
add_library(
    binfo_lib STATIC
    src/inspect.cpp
    src/detect.cpp
)
target_include_directories(binfo_lib PUBLIC include)

# Main binary
add_executable(binfo src/main.cpp)
target_link_libraries(binfo PRIVATE binfo_lib)

# Tests
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

enable_testing()

add_executable(binfo_tests tests/test_detect.cpp)
target_link_libraries(binfo_tests PRIVATE binfo_lib Catch2::Catch2WithMain)

add_test(NAME binfo_tests COMMAND binfo_tests)
