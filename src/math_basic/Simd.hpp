#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/buffer_info.h>
#include <cstring>
#include <vector>

namespace py = pybind11;

// Cross-platform SIMD setup
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #include <immintrin.h>
    #define SIMD_TYPE __m128
    #define SIMD_SETZERO _mm_setzero_ps()
    #define SIMD_SET(w,z,y,x) _mm_set_ps(w,z,y,x)
    #define SIMD_ADD(a,b) _mm_add_ps(a,b)
    #define SIMD_MUL(a,b) _mm_mul_ps(a,b)
    #define SIMD_SET1(val) _mm_set1_ps(val)
    
    inline float simd_hadd(__m128 a) {
        // Horizontal add for dot products
        __m128 shuf = _mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 3, 0, 1));
        __m128 sums = _mm_add_ps(a, shuf);
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
    #define SIMD_MUL(a,b) vmulq_f32(a,b)
    #define SIMD_SET1(val) vdupq_n_f32(val)
    
    inline float simd_hadd(float32x4_t a) {
        float32x2_t sum = vpadd_f32(vget_low_f32(a), vget_high_f32(a));
        sum = vpadd_f32(sum, sum);
        return vget_lane_f32(sum, 0);
    }
#endif

// Pure C++ Complex Multiplication (used for matrix internals)
struct Complex {
    float real;
    float imag;
    
    Complex(float r = 0.0f, float i = 0.0f) : real(r), imag(i) {}
    
    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }
    
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
};