/**
 * @file HistogramOps.hpp
 * @brief Histogram based image enhancements.
 * 
 * Includes standard Histogram Equalization and Contrast Limited Adaptive
 * Histogram Equalization (CLAHE) for local contrast enhancement.
 */
#pragma once
#include "../core/NdArray.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

namespace qmath {

inline NdArray<float> histogram_equalize(const NdArray<float>& image) {
    size_t size = image.size();
    std::vector<float> sorted_data(size);
    for (size_t i = 0; i < size; ++i) sorted_data[i] = image[i];
    std::sort(sorted_data.begin(), sorted_data.end());
    
    NdArray<float> result(image.shape());
    for (size_t i = 0; i < size; ++i) {
        float val = image[i];
        auto it = std::lower_bound(sorted_data.begin(), sorted_data.end(), val);
        size_t rank = it - sorted_data.begin();
        result[i] = static_cast<float>(rank) / static_cast<float>(size - 1);
    }
    return result;
}

// CLAHE (Simplified implementation for 8-bit equivalent images normalized to [0,1])
inline NdArray<float> clahe(const NdArray<float>& image, size_t tile_size = 8, float clip_limit = 0.01f) {
    NdArray<float> result(image.shape());
    size_t rows = image.shape()[0];
    size_t cols = image.shape()[1];
    
    size_t num_tiles_x = cols / tile_size;
    size_t num_tiles_y = rows / tile_size;
    
    std::vector<std::vector<float>> luts(num_tiles_y * num_tiles_x);
    
    for (size_t ty = 0; ty < num_tiles_y; ++ty) {
        for (size_t tx = 0; tx < num_tiles_x; ++tx) {
            std::vector<size_t> hist(256, 0);
            for (size_t i = 0; i < tile_size; ++i) {
                for (size_t j = 0; j < tile_size; ++j) {
                    size_t r = ty * tile_size + i;
                    size_t c = tx * tile_size + j;
                    size_t bin = static_cast<size_t>(image.at({r, c}) * 255.0f);
                    if (bin > 255) bin = 255;
                    hist[bin]++;
                }
            }
            
            // Clip histogram
            size_t limit = static_cast<size_t>(clip_limit * tile_size * tile_size);
            size_t excess = 0;
            for (size_t i = 0; i < 256; ++i) {
                if (hist[i] > limit) {
                    excess += hist[i] - limit;
                    hist[i] = limit;
                }
            }
            // Redistribute excess
            size_t redistrib = excess / 256;
            for (size_t i = 0; i < 256; ++i) hist[i] += redistrib;
            
            // Create LUT (CDF)
            std::vector<float> lut(256);
            size_t sum = 0;
            for (size_t i = 0; i < 256; ++i) {
                sum += hist[i];
                lut[i] = static_cast<float>(sum) / (tile_size * tile_size);
            }
            luts[ty * num_tiles_x + tx] = lut;
        }
    }
    
    // Bilinear interpolation of LUTs for final image
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            float fx = static_cast<float>(j) / tile_size;
            float fy = static_cast<float>(i) / tile_size;
            
            size_t tx0 = std::min(static_cast<size_t>(fx), num_tiles_x - 1);
            size_t ty0 = std::min(static_cast<size_t>(fy), num_tiles_y - 1);
            size_t tx1 = std::min(tx0 + 1, num_tiles_x - 1);
            size_t ty1 = std::min(ty0 + 1, num_tiles_y - 1);
            
            float wx = fx - tx0;
            float wy = fy - ty0;
            
            size_t bin = static_cast<size_t>(image.at({i, j}) * 255.0f);
            if (bin > 255) bin = 255;
            
            float v00 = luts[ty0 * num_tiles_x + tx0][bin];
            float v01 = luts[ty0 * num_tiles_x + tx1][bin];
            float v10 = luts[ty1 * num_tiles_x + tx0][bin];
            float v11 = luts[ty1 * num_tiles_x + tx1][bin];
            
            float v0 = v00 * (1.0f - wx) + v01 * wx;
            float v1 = v10 * (1.0f - wx) + v11 * wx;
            result.at({i, j}) = v0 * (1.0f - wy) + v1 * wy;
        }
    }
    return result;
}

} // namespace qmath
