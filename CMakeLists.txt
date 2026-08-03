cmake_minimum_required(VERSION 3.18)
project(qmath LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.12.0
)
FetchContent_MakeAvailable(pybind11)

# Apply SSE4.2 ONLY on x86_64 (Intel/AMD). 
# This prevents macOS Apple Silicon (ARM64) from crashing!
if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|amd64|AMD64|i386|i686)")
    add_compile_options(-msse4.2 -O3)
else()
    add_compile_options(-O3)
endif()

pybind11_add_module(qmath 
    src/math_basic/vector.cpp
)

# CRITICAL: Tell CMake to put the compiled .so/.pyd inside the math_basic package folder
set_target_properties(qmath PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/math_basic"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/math_basic"
)