#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/buffer_info.h>
#include <cstring>

namespace py = pybind11;

// Cross-platform SIMD setup
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
    #include <immintrin.h>
    
    // Multiplies two complex numbers packed in a 128-bit register: [c0, c1, c2, c3]
    // Where c0 = ar+ai, c1 = br+bi, c2 = cr+ci, c3 = dr+di
    // Note: This is a simplified 2-complex multiply using SSE
    inline __m128 complex_mul_sse(__m128 a, __m128 b) {
        // a = [a.r, a.i, b.r, b.i]
        // b = [c.r, c.i, d.r, d.i]
        __m128 a_r = _mm_moveldup_ps(a); // [a.r, a.r, b.r, b.r]
        __m128 a_i = _mm_movehdup_ps(a); // [a.i, a.i, b.i, b.i]
        __m128 b_r = _mm_moveldup_ps(b); // [c.r, c.r, d.r, d.r]
        __m128 b_i = _mm_movehdup_ps(b); // [c.i, c.i, d.i, d.i]
        
        __m128 real_part = _mm_mul_ps(a_r, b_r);
        __m128 imag_part = _mm_mul_ps(a_i, b_i);
        
        // real = a.r*c.r - a.i*c.i
        __m128 real = _mm_addsub_ps(real_part, imag_part); // Subtracts the second pair!
        
        __m128 cross1 = _mm_mul_ps(a_r, b_i);
        __m128 cross2 = _mm_mul_ps(a_i, b_r);
        // imag = a.r*c.i + a.i*c.r
        __m128 imag = _mm_add_ps(cross1, cross2);
        
        // Interleave back to [real0, imag0, real1, imag1]
        return _mm_shuffle_ps(real, imag, _MM_SHUFFLE(2, 0, 2, 0)); // Rough shuffle, needs refinement based on layout
    }

#elif defined(__aarch64__) || defined(_M_ARM64) || defined(__arm__)
    #include <arm_neon.h>
    // ARM NEON complex multiplication fallback
    inline float32x4_t complex_mul_neon(float32x4_t a, float32x4_t b) {
        // Simplified fallback for ARM
        float32x4_t a_r = vdupq_n_f32(0.0f); // Placeholder for ARM logic
        return vmulq_f32(a, b);
    }
#endif