#include <pybind11/pybind11.h>
#include <pybind11/buffer_info.h>
#include <cstring>

namespace py = pybind11;

// Cross-platform SIMD setup
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #include <immintrin.h>
    #define SIMD_TYPE __m128
    #define SIMD_SETZERO _mm_setzero_ps()
    #define SIMD_SET(w,z,y,x) _mm_set_ps(w,z,y,x)
    #define SIMD_ADD(a,b) _mm_add_ps(a,b)
    inline float simd_dot(__m128 a, __m128 b) {
        __m128 mul = _mm_mul_ps(a, b);
        __m128 shuf = _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(mul, shuf);
        shuf = _mm_movehl_ps(shuf, sums);
        sums = _mm_add_ss(sums, shuf);
        return _mm_cvtss_f32(sums);
    }
#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__)
    #include <arm_neon.h>
    #define SIMD_TYPE float32x4_t
    #define SIMD_SETZERO vdupq_n_f32(0.0f)
    #define SIMD_SET(w,z,y,x) float32x4_t{x,y,z,w}
    #define SIMD_ADD(a,b) vaddq_f32(a,b)
    inline float simd_dot(float32x4_t a, float32x4_t b) {
        float32x4_t mul = vmulq_f32(a, b);
        float32x2_t sum = vpadd_f32(vget_low_f32(mul), vget_high_f32(mul));
        sum = vpadd_f32(sum, sum);
        return vget_lane_f32(sum, 0);
    }
#endif

class Vec4 {
private:
    SIMD_TYPE data;

public:
    Vec4() : data(SIMD_SETZERO) {}
    Vec4(float x, float y, float z, float w) : data(SIMD_SET(w, z, y, x)) {}
    Vec4(SIMD_TYPE v) : data(v) {}

    Vec4 operator+(const Vec4& other) const {
        return Vec4(SIMD_ADD(data, other.data));
    }

    float dot(const Vec4& other) const {
        return simd_dot(data, other.data);
    }

    py::buffer_info get_buffer_info() {
        return py::buffer_info(
            &data,
            sizeof(float),
            py::format_descriptor<float>::format(),
            1,
            { 4 },
            { sizeof(float) }
        );
    }
};

PYBIND11_MODULE(qmath, m) {
    m.doc() = "Bare-metal SIMD-accelerated math library";

    py::class_<Vec4>(m, "Vec4", py::buffer_protocol())
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        .def_buffer([](Vec4 &v) -> py::buffer_info {
            return v.get_buffer_info();
        })
        .def("__add__", &Vec4::operator+, py::is_operator())
        .def("dot", &Vec4::dot);
}