/**
 * @file Convolve.hpp
 * @brief 1D and 2D Convolution implementations.
 * 
 * Used extensively in signal processing (filters) and image processing (blurs, edge detection).
 */
#pragma once
#include "../core/NdArray.hpp"
#include "../core/Error.hpp"
#include <vector>

namespace qmath {

// 1D Convolution (Same size output)
inline NdArray<float> convolve1d(const NdArray<float>& signal, const NdArray<float>& kernel) {
    if (signal.ndim() != 1 || kernel.ndim() != 1) throw DimensionError("Inputs must be 1D");
    
    size_t n = signal.size();
    size_t k = kernel.size();
    if (k == 0 || n == 0) throw DimensionError("Inputs cannot be empty");
    
    size_t half_k = k / 2;
    NdArray<float> result({n});
    
    for (size_t i = 0; i < n; ++i) {
        float sum = 0.0f;
        for (size_t j = 0; j < k; ++j) {
            int signal_idx = static_cast<int>(i + j - half_k);
            // Zero padding for boundaries
            if (signal_idx >= 0 && signal_idx < static_cast<int>(n)) {
                sum += signal[signal_idx] * kernel[j];
            }
        }
        result[i] = sum;
    }
    return result;
}

// 2D Convolution (Same size output)
inline NdArray<float> convolve2d(const NdArray<float>& image, const NdArray<float>& kernel) {
    if (image.ndim() != 2 || kernel.ndim() != 2) throw DimensionError("Inputs must be 2D");
    
    auto img_shape = image.shape();
    auto ker_shape = kernel.shape();
    size_t rows = img_shape[0];
    size_t cols = img_shape[1];
    size_t k_rows = ker_shape[0];
    size_t k_cols = ker_shape[1];
    
    size_t half_k_rows = k_rows / 2;
    size_t half_k_cols = k_cols / 2;
    
    NdArray<float> result({rows, cols});
    
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            float sum = 0.0f;
            for (size_t ki = 0; ki < k_rows; ++ki) {
                for (size_t kj = 0; kj < k_cols; ++kj) {
                    int img_r = static_cast<int>(i + ki - half_k_rows);
                    int img_c = static_cast<int>(j + kj - half_k_cols);
                    // Zero padding for boundaries
                    if (img_r >= 0 && img_r < static_cast<int>(rows) && img_c >= 0 && img_c < static_cast<int>(cols)) {
                        sum += image.at({static_cast<size_t>(img_r), static_cast<size_t>(img_c)}) * kernel.at({ki, kj});
                    }
                }
            }
            result.at({i, j}) = sum;
        }
    }
    return result;
}

} // namespace qmath
