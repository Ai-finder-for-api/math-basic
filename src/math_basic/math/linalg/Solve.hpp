/**
 * @file Solve.hpp
 * @brief Solves a system of linear equations (Ax = b).
 * 
 * Uses forward and backward substitution following LU decomposition.
 */
#pragma once
#include "LU.hpp"
#include <vector>

namespace qmath {

template<typename T>
std::vector<T> solve(const DMatrix<T>& A, const std::vector<T>& b) {
    if (A.rows() != A.cols()) throw std::runtime_error("Solve requires a square matrix");
    size_t n = A.rows();
    if (b.size() != n) throw std::runtime_error("Dimension mismatch between A and b");

    LU_Result<T> lu = lu_decompose(A);

    // Apply permutation to b
    std::vector<T> Pb(n);
    for (size_t i = 0; i < n; ++i) {
        Pb[i] = b[lu.P[i]];
    }

    // Forward substitution: Ly = Pb
    std::vector<T> y(n);
    for (size_t i = 0; i < n; ++i) {
        T sum = Pb[i];
        for (size_t j = 0; j < i; ++j) {
            sum -= lu.L(i, j) * y[j];
        }
        y[i] = sum / lu.L(i, i);
    }

    // Backward substitution: Ux = y
    std::vector<T> x(n);
    for (int i = n - 1; i >= 0; --i) {
        T sum = y[i];
        for (size_t j = i + 1; j < n; ++j) {
            sum -= lu.U(i, j) * x[j];
        }
        x[i] = sum / lu.U(i, i);
    }

    return x;
}

} // namespace qmath
