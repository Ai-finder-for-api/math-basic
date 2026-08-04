/**
 * @file FFT.hpp
 * @brief Fast Fourier Transform implementation.
 * 
 * Implements the radix-2 Cooley-Tukey FFT algorithm for arrays of size 2^n.
 * Crucial for signal processing and frequency domain analysis.
 */
#pragma once
#include "../core/NdArray.hpp"
#include "../core/Complex.hpp"
#include "../core/Error.hpp"
#include <cmath>

namespace qmath {

// In-place Radix-2 FFT
inline void fft_radix2(NdArray<Complex<float>>& a) {
    size_t n = a.size();
    if (n == 0) return;
    
    // Check if n is a power of 2
    if ((n & (n - 1)) != 0) {
        throw DimensionError("FFT array size must be a power of 2");
    }

    // Bit reversal permutation
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(a[i], a[j]);
        }
    }

    // Cooley-Tukey iterative FFT
    for (size_t len = 2; len <= n; len <<= 1) {
        float ang = -2.0f * PI_F / static_cast<float>(len);
        Complex<float> wlen(std::cos(ang), std::sin(ang));
        
        for (size_t i = 0; i < n; i += len) {
            Complex<float> w(1.0f, 0.0f);
            for (size_t j = 0; j < len / 2; ++j) {
                Complex<float> u = a[i + j];
                Complex<float> v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w = w * wlen;
            }
        }
    }
}

// Inverse FFT
inline void ifft_radix2(NdArray<Complex<float>>& a) {
    size_t n = a.size();
    if (n == 0) return;
    
    // Conjugate
    for (size_t i = 0; i < n; ++i) {
        a[i].imag = -a[i].imag;
    }
    
    // Apply FFT
    fft_radix2(a);
    
    // Conjugate and scale by 1/n
    float inv_n = 1.0f / static_cast<float>(n);
    for (size_t i = 0; i < n; ++i) {
        a[i].imag = -a[i].imag;
        a[i].real *= inv_n;
        a[i].imag *= inv_n;
    }
}

} // namespace qmath
