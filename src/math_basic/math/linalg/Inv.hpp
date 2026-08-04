/**
 * @file Inv.hpp
 * @brief Matrix Inversion using Gauss-Jordan elimination.
 */
#pragma once
#include "Matrix.hpp"
#include "../core/Utils.hpp"
#include <stdexcept>
#include <algorithm>

namespace qmath {

template<typename T>
DMatrix<T> inverse(const DMatrix<T>& A) {
    if (A.rows() != A.cols()) throw std::runtime_error("Inverse requires a square matrix");
    size_t n = A.rows();

    // Create augmented matrix [A | I]
    DMatrix<T> aug(n, 2 * n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            aug(i, j) = A(i, j);
        }
        aug(i, i + n) = static_cast<T>(1); // Identity matrix
    }

    // Perform Gauss-Jordan elimination
    for (size_t i = 0; i < n; ++i) {
        // Find pivot
        size_t pivot = i;
        T max_val = std::fabs(aug(i, i));
        for (size_t k = i + 1; k < n; ++k) {
            if (std::fabs(aug(k, i)) > max_val) {
                max_val = std::fabs(aug(k, i));
                pivot = k;
            }
        }

        if (max_val < static_cast<T>(1e-12)) throw std::runtime_error("Matrix is singular and cannot be inverted");

        // Swap rows
        if (pivot != i) {
            for (size_t j = 0; j < 2 * n; ++j) {
                std::swap(aug(i, j), aug(pivot, j));
            }
        }

        // Scale pivot row to make pivot 1
        T pivot_val = aug(i, i);
        for (size_t j = 0; j < 2 * n; ++j) {
            aug(i, j) /= pivot_val;
        }

        // Eliminate other rows
        for (size_t k = 0; k < n; ++k) {
            if (k != i) {
                T factor = aug(k, i);
                for (size_t j = 0; j < 2 * n; ++j) {
                    aug(k, j) -= factor * aug(i, j);
                }
            }
        }
    }

    // Extract the right half (the inverse)
    DMatrix<T> inv(n, n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            inv(i, j) = aug(i, j + n);
        }
    }

    return inv;
}

} // namespace qmath
