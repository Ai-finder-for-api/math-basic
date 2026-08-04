/**
 * @file Kernels.hpp
 * @brief Standard Image Processing Kernels.
 * 
 * Generates convolution matrices for common image processing tasks
 * such as blurring, edge detection, and sharpening.
 */
#pragma once
#include "../linalg/Matrix.hpp"
#include <cmath>

namespace qmath {

// Gaussian Blur Kernel
inline DMatrix<float> gaussian_kernel(size_t size, float sigma) {
    if (size % 2 == 0) size++; // Must be odd
    DMatrix<float> k(size, size);
    int center = size / 2;
    float sum = 0.0f;
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int x = i - center;
            int y = j - center;
            float val = std::exp(-(x*x + y*y) / (2.0f * sigma * sigma));
            k(i, j) = val;
            sum += val;
        }
    }
    
    // Normalize
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            k(i, j) /= sum;
        }
    }
    return k;
}

// Sobel Operators for Edge Detection
inline DMatrix<float> sobel_x() {
    return DMatrix<float>(3, 3, {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    });
}

inline DMatrix<float> sobel_y() {
    return DMatrix<float>(3, 3, {
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    });
}

// Laplacian Operator (Edge Detection)
inline DMatrix<float> laplacian() {
    return DMatrix<float>(3, 3, {
        0,  1, 0,
        1, -4, 1,
        0,  1, 0
    });
}

// Sharpening Kernel
inline DMatrix<float> sharpen() {
    return DMatrix<float>(3, 3, {
         0, -1,  0,
        -1,  5, -1,
         0, -1,  0
    });
}

// Emboss Kernel
inline DMatrix<float> emboss() {
    return DMatrix<float>(3, 3, {
        -2, -1, 0,
        -1,  1, 1,
         0,  1, 2
    });
}

// Box Blur (Average)
inline DMatrix<float> box_blur(size_t size) {
    if (size % 2 == 0) size++;
    DMatrix<float> k(size, size);
    float val = 1.0f / static_cast<float>(size * size);
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            k(i, j) = val;
        }
    }
    return k;
}

} // namespace qmath
