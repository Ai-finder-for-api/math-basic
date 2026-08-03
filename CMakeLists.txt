cmake_minimum_required(VERSION 3.18)
project(qmath LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch pybind11 automatically (no external deps required to be installed)
include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.12.0
)
FetchContent_MakeAvailable(pybind11)

# Enable AVX2/SSE4.2 if the host machine supports it
add_compile_options(-march=native -O3)

# Create the Python extension module
pybind11_add_module(qmath 
    src/qmath/vector.cpp
)