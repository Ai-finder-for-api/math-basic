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

if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|amd64|AMD64|i386|i686)")
    add_compile_options(-msse4.2 -O3)
else()
    add_compile_options(-O3)
endif()

file(GLOB QMATH_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/math_basic/bindings/*.cpp")

pybind11_add_module(qmath ${QMATH_SOURCES})
target_include_directories(qmath PRIVATE 
    src/math_basic/core 
    src/math_basic/math/3d 
    src/math_basic/math/color 
    src/math_basic/math/quantum 
    src/math_basic/math/linalg 
    src/math_basic/math/random
    src/math_basic/math/processing
    src/math_basic/math/stats
    src/math_basic/math/physics
    src/math_basic/math/crypto
    src/math_basic/math/ml
    src/math_basic/math/geometry
    src/math_basic/math/procgen
    src/math_basic/math/ode
    src/math_basic/math/graph
    src/math_basic/math/compression
)
install(TARGETS qmath DESTINATION math_basic)
