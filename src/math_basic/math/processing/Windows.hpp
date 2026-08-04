/**
 * @file Windows.hpp
 * @brief Digital Signal Processing Window Functions.
 * 
 * Used to reduce spectral leakage when performing Fourier Transforms.
 * Includes Hamming, Hann, Blackman, Blackman-Harris, and Flat-top windows.
 */
#pragma once
#include "../core/NdArray.hpp"
#include "../core/Constants.hpp"
#include <cmath>

namespace qmath {

enum class WindowType {
    HAMMING,
    HANN,
    BLACKMAN,
    BLACKMAN_HARRIS,
    FLAT_TOP,
    KAISER
};

inline NdArray<float> generate_window(size_t N, WindowType type) {
    NdArray<float> window({N});
    if (N == 0) return window;
    
    for (size_t i = 0; i < N; ++i) {
        float n = static_cast<float>(i);
        float x = n / static_cast<float>(N - 1);
        float val = 0.0f;
        
        switch (type) {
            case WindowType::HAMMING:
                val = 0.54f - 0.46f * std::cos(2.0f * PI_F * x);
                break;
            case WindowType::HANN:
                val = 0.5f * (1.0f - std::cos(2.0f * PI_F * x));
                break;
            case WindowType::BLACKMAN:
                val = 0.42f - 0.5f * std::cos(2.0f * PI_F * x) + 0.08f * std::cos(4.0f * PI_F * x);
                break;
            case WindowType::BLACKMAN_HARRIS:
                val = 0.35875f 
                    - 0.48829f * std::cos(2.0f * PI_F * x) 
                    + 0.14128f * std::cos(4.0f * PI_F * x)
                    - 0.01168f * std::cos(6.0f * PI_F * x);
                break;
            case WindowType::FLAT_TOP:
                val = 0.21557895f 
                    - 0.41663158f * std::cos(2.0f * PI_F * x) 
                    + 0.277263158f * std::cos(4.0f * PI_F * x)
                    - 0.083578947f * std::cos(6.0f * PI_F * x)
                    + 0.006947368f * std::cos(8.0f * PI_F * x);
                break;
            default:
                val = 1.0f; // Rectangular
                break;
        }
        window[i] = val;
    }
    return window;
}

// Kaiser-Bessel window (requires beta parameter)
inline NdArray<float> kaiser_window(size_t N, float beta) {
    NdArray<float> window({N});
    if (N == 0) return window;
    
    // Modified Bessel function of the first kind, order 0 (approximation)
    auto bessel_i0 = [](float x) {
        float sum = 1.0f;
        float term = 1.0f;
        for (int k = 1; k < 20; ++k) {
            term *= (x * x) / (4.0f * k * k);
            sum += term;
            if (term < 1e-7f * sum) break;
        }
        return sum;
    };
    
    float denom = bessel_i0(beta);
    for (size_t i = 0; i < N; ++i) {
        float x = static_cast<float>(i) / static_cast<float>(N - 1) - 0.5f;
        float arg = beta * std::sqrt(1.0f - 4.0f * x * x);
        window[i] = bessel_i0(arg) / denom;
    }
    return window;
}

} // namespace qmath
