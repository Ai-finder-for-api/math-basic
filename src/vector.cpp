#include <pybind11/pybind11.h>
#include <pybind11/buffer_info.h>
#include <immintrin.h>
#include <cstring>

namespace py = pybind11;

class Vec4 {
private:
    __m128 data;

public:
    // Constructors
    Vec4() : data(_mm_setzero_ps()) {}
    Vec4(float x, float y, float z, float w) : data(_mm_set_ps(w, z, y, x)) {}
    
    // Raw __m128 constructor for internal math
    Vec4(__m128 v) : data(v) {}

    // SIMD-accelerated addition
    Vec4 operator+(const Vec4& other) const {
        return Vec4(_mm_add_ps(data, other.data));
    }

    // SIMD-accelerated dot product
    float dot(const Vec4& other) const {
        __m128 mul = _mm_mul_ps(data, other.data);
        // Horizontal add (hadd) to sum the 4 components
        __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(mul, shuf);
        shuf = _mm_movehl_ps(shuf, sums);
        sums = _mm_add_ss(sums, shuf);
        return _mm_cvtss_f32(sums);
    }

    // Implement Python Buffer Protocol for zero-copy memory access
    py::buffer_info get_buffer_info() {
        return py::buffer_info(
            &data,                               // Pointer to memory
            sizeof(float),                       // Size of one scalar
            py::format_descriptor<float>::format(), // Format descriptor
            1,                                   // Number of dimensions
            { 4 },                               // Buffer dimensions
            { sizeof(float) }                    // Strides (in bytes)
        );
    }
};

PYBIND11_MODULE(qmath, m) {
    m.doc() = "Bare-metal SIMD-accelerated math library";

    // Bind the buffer protocol to Vec4
    py::class_<Vec4>(m, "Vec4", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        
        // Expose buffer protocol so Python can read/write the raw memory
        .def_buffer([](Vec4 &v) -> py::buffer_info {
            return v.get_buffer_info();
        })
        
        // Math operations
        .def("__add__", &Vec4::operator+, py::is_operator())
        .def("dot", &Vec4::dot);
}