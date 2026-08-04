/**
 * @file PyFastMath.cpp
 * @brief Ultra-fast array operations that accept raw NumPy arrays.
 * 
 * This bypasses Python list overhead entirely, operating on raw C pointers.
 */
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "../math/3d/Vec4.hpp"
#include "../math/3d/Vec3.hpp"

namespace py = pybind11;
using namespace qmath;

// Processes an (N, 4) NumPy array of vectors.
// Multiplies by 2, then dots with itself. Returns an (N,) array.
py::array_t<float> fast_batch_process(py::array_t<float, py::array::c_style | py::array::forcecast> input) {
    auto buf = input.request();
    if (buf.ndim != 2 || buf.shape[1] != 4) {
        throw std::runtime_error("Input must be an (N, 4) array");
    }
    
    size_t N = buf.shape[0];
    py::array_t<float> result(N);
    auto res_buf = result.request();
    
    float* in_ptr = static_cast<float*>(buf.ptr);
    float* out_ptr = static_cast<float*>(res_buf.ptr);
    
    // Pure C++ loop with zero Python overhead.
    // The compiler will auto-vectorize this using AVX/SSE due to -O3 flags.
    #pragma omp parallel for
    for (size_t i = 0; i < N; ++i) {
        float* v_ptr = in_ptr + i * 4;
        
        // Reinterpret memory directly as our Vec4f SIMD type
        Vec4f v = *reinterpret_cast<Vec4f*>(v_ptr);
        
        // Do the math
        Vec4f scaled = v * 2.0f;
        out_ptr[i] = scaled.dot(scaled);
    }
    
    return result;
}

void init_fast_math(py::module_ &m) {
    m.def("fast_batch_process", &fast_batch_process, "Ultra-fast batch processing on raw NumPy arrays");
}
