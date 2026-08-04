/**
 * @file LU.hpp
 * @brief LU Decomposition with partial pivoting.
 * 
 * Decomposes a matrix A into L (lower triangular) and U (upper triangular)
 * such that P*A = L*U, where P is a permutation matrix.
 */
#pragma once
#include "Matrix.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

namespace qmath {

template<typename T>
struct LU_Result {
    DMatrix<T> L;
    DMatrix<T> U;
    std::vector<size_t> P; // Permutation vector
    int sign; // +1 or -1 (for determinant calculation)
};

template<typename T>
LU_Result<T> lu_decompose(const DMatrix<T>& A) {
    if (A.rows() != A.cols()) throw std::runtime_error("LU decomposition requires a square matrix");
    size_t n = A.rows();
    
    LU_Result<T> res;
    res.L = DMatrix<T>(n, n);
    res.U = DMatrix<T>(n, n);
    res.P.resize(n);
    res.sign = 1;
    
    for (size_t i = 0; i < n; ++i) res.P[i] = i;

    // Copy A to U
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            res.U(i, j) = A(i, j);
        }
    }

    for (size_t k = 0; k < n; ++k) {
        // Partial Pivoting: Find the row with the maximum element in the current column
        size_t max_row = k;
        T max_val = std::fabs(res.U(k, k));
        for (size_t i = k + 1; i < n; ++i) {
            if (std::fabs(res.U(i, k)) > max_val) {
                max_val = std::fabs(res.U(i, k));
                max_row = i;
            }
        }

        if (max_val < static_cast<T>(1e-12)) {
            throw std::runtime_error("Matrix is singular");
        }

        // Swap rows in U, L, and P
        if (max_row != k) {
            res.P[k] ^= res.P[max_row];
            res.P[max_row] ^= res.P[k];
            res.P[k] ^= res.P[max_row];
            res.sign *= -1;

            for (size_t j = 0; j < n; ++j) {
                std::swap(res.U(k, j), res.U(max_row, j));
                std::swap(res.L(k, j), res.L(max_row, j));
            }
        }

        // Doolittle algorithm: compute L and U
        for (size_t i = k + 1; i < n; ++i) {
            res.L(i, k) = res.U(i, k) / res.U(k, k);
            for (size_t j = k; j < n; ++j) {
                res.U(i, j) -= res.L(i, k) * res.U(k, j);
            }
        }
    }

    // Set diagonal of L to 1
    for (size_t i = 0; i < n; ++i) res.L(i, i) = static_cast<T>(1);

    return res;
}

} // namespace qmath
