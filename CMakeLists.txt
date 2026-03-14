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

add_executable(
    binfo
    src/main.cpp
    src/inspect.cpp
)
