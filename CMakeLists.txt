cmake_minimum_required(VERSION 3.18)
project(qmath LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch pybind11 automatically
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.12.0
)
FetchContent_MakeAvailable(pybind11)

# Use standard SSE4.2 (Safe for GitHub Actions runners)
add_compile_options(-msse4.2 -O3)
# Note: If you are on an older CPU that doesn't support SSE4.2, remove the line above temporarily.

# Point to the correct C++ file location
pybind11_add_module(qmath 
    src/vector.cpp
)