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
    # Enable AVX, AVX2, SSE4.2, and FMA (Fused Multiply-Add)
    add_compile_options(-mavx -mavx2 -msse4.2 -mfma -O3)
else()
    add_compile_options(-O3)
endif()

pybind11_add_module(qmath 
    src/math_basic/vector.cpp
    src/math_basic/matrix.cpp
)

# Add the math_basic directory to the include path so .cpp files can find .hpp files
target_include_directories(qmath PRIVATE src/math_basic)

install(TARGETS qmath DESTINATION math_basic)