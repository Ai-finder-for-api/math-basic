/**
 * @file Morphology.hpp
 * @brief Mathematical Morphology for Image Processing.
 * 
 * Implements Erosion, Dilation, Opening, and Closing using structuring elements.
 * Used for noise removal, shape analysis, and feature extraction.
 */
#pragma once
#include "../core/NdArray.hpp"
#include <vector>
#include <algorithm>

namespace qmath {

struct StructuringElement {
    std::vector<std::pair<int, int>> offsets;
};

inline StructuringElement square_element(size_t size) {
    StructuringElement se;
    int half = static_cast<int>(size / 2);
    for (int i = -half; i <= half; ++i) {
        for (int j = -half; j <= half; ++j) {
            se.offsets.push_back({i, j});
        }
    }
    return se;
}

inline StructuringElement cross_element(size_t size) {
    StructuringElement se;
    int half = static_cast<int>(size / 2);
    for (int i = -half; i <= half; ++i) {
        se.offsets.push_back({i, 0});
        se.offsets.push_back({0, i});
    }
    return se;
}

inline NdArray<float> erode(const NdArray<float>& image, const StructuringElement& se) {
    NdArray<float> result(image.shape());
    size_t rows = image.shape()[0];
    size_t cols = image.shape()[1];
    
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            float min_val = std::numeric_limits<float>::max();
            for (const auto& offset : se.offsets) {
                int ni = static_cast<int>(i) + offset.first;
                int nj = static_cast<int>(j) + offset.second;
                if (ni >= 0 && ni < static_cast<int>(rows) && nj >= 0 && nj < static_cast<int>(cols)) {
                    min_val = std::min(min_val, image.at({static_cast<size_t>(ni), static_cast<size_t>(nj)}));
                }
            }
            result.at({i, j}) = min_val;
        }
    }
    return result;
}

inline NdArray<float> dilate(const NdArray<float>& image, const StructuringElement& se) {
    NdArray<float> result(image.shape());
    size_t rows = image.shape()[0];
    size_t cols = image.shape()[1];
    
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            float max_val = std::numeric_limits<float>::lowest();
            for (const auto& offset : se.offsets) {
                int ni = static_cast<int>(i) + offset.first;
                int nj = static_cast<int>(j) + offset.second;
                if (ni >= 0 && ni < static_cast<int>(rows) && nj >= 0 && nj < static_cast<int>(cols)) {
                    max_val = std::max(max_val, image.at({static_cast<size_t>(ni), static_cast<size_t>(nj)}));
                }
            }
            result.at({i, j}) = max_val;
        }
    }
    return result;
}

inline NdArray<float> open(const NdArray<float>& image, const StructuringElement& se) {
    return dilate(erode(image, se), se);
}

inline NdArray<float> close(const NdArray<float>& image, const StructuringElement& se) {
    return erode(dilate(image, se), se);
}

} // namespace qmath
