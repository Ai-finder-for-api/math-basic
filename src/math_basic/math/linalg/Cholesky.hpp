/**
 * @file Cholesky.hpp
 * @brief Cholesky Decomposition for Symmetric Positive-Definite matrices.
 * 
 * Decomposes A into L * L^T. Twice as efficient as LU decomposition for
 * applicable matrices.
 */
#pragma once
#include "Matrix.hpp"
#include "../core/Error.hpp"
#include <cmath>
#include <stdexcept>

namespace qmath {

template<typename T>
struct Cholesky_Result {
    DMatrix<T> L;
};

template<typename T>
Cholesky_Result<T> cholesky_decompose(const DMatrix<T>& A) {
    if (A.rows() != A.cols()) throw DimensionError("Cholesky requires square matrix");
    size_t n = A.rows();
    DMatrix<T> L(n, n);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            T sum = A(i, j);
            
            for (size_t k = 0; k < j; ++k) {
                sum -= L(i, k) * L(j, k);
            }
            
            if (i == j) {
                if (sum <= static_cast<T>(0)) {
                    throw MathError("Matrix is not positive definite");
                }
                L(i, j) = std::sqrt(sum);
            } else {
                L(i, j) = sum / L(j, j);
            }
        }
    }
    return { L };
}

} // namespace qmath
