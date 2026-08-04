/**
 * @file Filters.hpp
 * @brief Digital Filter implementations (Biquad, FIR).
 * 
 * Implements standard audio and signal processing filters (Low-pass, High-pass,
 * Band-pass) using Direct Form I and II Transposed structures.
 */
#pragma once
#include "../core/NdArray.hpp"
#include "../core/Constants.hpp"
#include "Windows.hpp"
#include <cmath>
#include <vector>

namespace qmath {

enum class FilterType {
    LOW_PASS,
    HIGH_PASS,
    BAND_PASS,
    NOTCH,
    ALL_PASS
};

// Biquad Filter Coefficients (a0, a1, a2, b0, b1, b2)
struct BiquadCoeffs {
    float a0, a1, a2, b0, b1, b2;
};

// Calculate Biquad coefficients using the RBJ Audio EQ Cookbook formulas
inline BiquadCoeffs design_biquad(FilterType type, float sample_rate, float cutoff_freq, float Q) {
    BiquadCoeffs c;
    float w0 = 2.0f * PI_F * cutoff_freq / sample_rate;
    float cos_w0 = std::cos(w0);
    float sin_w0 = std::sin(w0);
    float alpha = sin_w0 / (2.0f * Q);
    
    float a0 = 0, a1 = 0, a2 = 0, b0 = 0, b1 = 0, b2 = 0;
    
    switch (type) {
        case FilterType::LOW_PASS:
            b0 = (1.0f - cos_w0) / 2.0f;
            b1 = 1.0f - cos_w0;
            b2 = (1.0f - cos_w0) / 2.0f;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cos_w0;
            a2 = 1.0f - alpha;
            break;
        case FilterType::HIGH_PASS:
            b0 = (1.0f + cos_w0) / 2.0f;
            b1 = -(1.0f + cos_w0);
            b2 = (1.0f + cos_w0) / 2.0f;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cos_w0;
            a2 = 1.0f - alpha;
            break;
        case FilterType::BAND_PASS:
            b0 = alpha;
            b1 = 0.0f;
            b2 = -alpha;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cos_w0;
            a2 = 1.0f - alpha;
            break;
        case FilterType::NOTCH:
            b0 = 1.0f;
            b1 = -2.0f * cos_w0;
            b2 = 1.0f;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cos_w0;
            a2 = 1.0f - alpha;
            break;
        case FilterType::ALL_PASS:
            b0 = 1.0f - alpha;
            b1 = -2.0f * cos_w0;
            b2 = 1.0f + alpha;
            a0 = 1.0f + alpha;
            a1 = -2.0f * cos_w0;
            a2 = 1.0f - alpha;
            break;
    }
    
    // Normalize by a0
    c.a0 = a0;
    c.a1 = a1 / a0;
    c.a2 = a2 / a0;
    c.b0 = b0 / a0;
    c.b1 = b1 / a0;
    c.b2 = b2 / a0;
    return c;
}

// Apply a Biquad filter to a 1D signal (Direct Form I)
inline NdArray<float> apply_biquad(const NdArray<float>& input, const BiquadCoeffs& c) {
    NdArray<float> output(input.shape());
    float x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    
    for (size_t i = 0; i < input.size(); ++i) {
        float x0 = input[i];
        float y0 = c.b0 * x0 + c.b1 * x1 + c.b2 * x2 - c.a1 * y1 - c.a2 * y2;
        output[i] = y0;
        x2 = x1;
        x1 = x0;
        y2 = y1;
        y1 = y0;
    }
    return output;
}

// Design a FIR filter using the Windowing method
inline std::vector<float> design_fir(FilterType type, size_t num_taps, float sample_rate, float cutoff_freq, WindowType window_type = WindowType::HAMMING) {
    if (num_taps % 2 == 0) num_taps++; // FIR filters should have odd number of taps for symmetric response
    std::vector<float> h(num_taps, 0.0f);
    NdArray<float> w = generate_window(num_taps, window_type);
    
    float fc = cutoff_freq / sample_rate;
    int M = (num_taps - 1) / 2;
    
    for (size_t i = 0; i < num_taps; ++i) {
        int n = static_cast<int>(i) - M;
        float val = 0.0f;
        
        if (n == 0) {
            val = 2.0f * fc;
        } else {
            val = std::sin(2.0f * PI_F * fc * n) / (PI_F * n);
        }
        
        if (type == FilterType::HIGH_PASS) {
            val = (n == 0) ? 1.0f - val : -val;
        }
        
        h[i] = val * w[i];
    }
    
    // Normalize filter coefficients
    float sum = 0.0f;
    for (float v : h) sum += v;
    for (float& v : h) v /= sum;
    
    return h;
}

} // namespace qmath
