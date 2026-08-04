/**
 * @file SVD.hpp
 * @brief Singular Value Decomposition using One-Sided Jacobi rotations.
 * 
 * Decomposes a matrix A into U * S * V^T.
 * Highly accurate and efficient for small to medium matrices.
 */
#pragma once
#include "Matrix.hpp"
#include "../core/Utils.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

namespace qmath {

template<typename T>
struct SVD_Result {
    DMatrix<T> U;
    DMatrix<T> S; // Diagonal matrix
    DMatrix<T> Vt; // V transposed
};

template<typename T>
SVD_Result<T> svd_jacobi(const DMatrix<T>& A, size_t max_iter = 100, T tolerance = static_cast<T>(1e-10)) {
    size_t m = A.rows();
    size_t n = A.cols();
    
    DMatrix<T> U = A;
    DMatrix<T> V = DMatrix<T>::identity(n); // Requires identity() static method in Matrix.hpp
    // If identity() is not available, manually init:
    // DMatrix<T> V(n, n);
    // for(size_t i=0; i<n; ++i) V(i,i) = 1;

    for (size_t iter = 0; iter < max_iter; ++iter) {
        bool converged = true;
        
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                // Compute alpha and beta for columns i and j
                T alpha = 0, beta = 0, gamma = 0;
                for (size_t k = 0; k < m; ++k) {
                    alpha += U(k, i) * U(k, i);
                    beta += U(k, j) * U(k, j);
                    gamma += U(k, i) * U(k, j);
                }
                
                if (std::fabs(gamma) < tolerance * std::sqrt(alpha * beta)) continue;
                converged = false;
                
                // Compute Jacobi rotation
                T zeta = (beta - alpha) / (2.0 * gamma);
                T t = (zeta >= 0) ? 1.0 / (std::sqrt(1.0 + zeta * zeta) + zeta) : -1.0 / (std::sqrt(1.0 + zeta * zeta) - zeta);
                T c = 1.0 / std::sqrt(1.0 + t * t);
                T s = c * t;
                
                // Apply rotation to columns of U and V
                for (size_t k = 0; k < m; ++k) {
                    T u_ik = U(k, i);
                    T u_jk = U(k, j);
                    U(k, i) = c * u_ik - s * u_jk;
                    U(k, j) = s * u_ik + c * u_jk;
                }
                
                for (size_t k = 0; k < n; ++k) {
                    T v_ik = V(k, i);
                    T v_jk = V(k, j);
                    V(k, i) = c * v_ik - s * v_jk;
                    V(k, j) = s * v_ik + c * v_jk;
                }
            }
        }
        if (converged) break;
    }
    
    // Extract singular values and normalize U
    std::vector<std::pair<T, size_t>> singular_values(n);
    for (size_t i = 0; i < n; ++i) {
        T sigma = 0;
        for (size_t k = 0; k < m; ++k) {
            sigma += U(k, i) * U(k, i);
        }
        sigma = std::sqrt(sigma);
        singular_values[i] = {sigma, i};
        
        if (sigma > static_cast<T>(1e-12)) {
            for (size_t k = 0; k < m; ++k) {
                U(k, i) /= sigma;
            }
        }
    }
    
    // Sort singular values in descending order
    std::sort(singular_values.begin(), singular_values.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });
    
    DMatrix<T> S(n, n);
    DMatrix<T> U_sorted(m, n);
    DMatrix<T> V_sorted(n, n);
    
    for (size_t i = 0; i < n; ++i) {
        T sigma = singular_values[i].first;
        size_t orig_idx = singular_values[i].second;
        
        S(i, i) = sigma;
        for (size_t k = 0; k < m; ++k) U_sorted(k, i) = U(k, orig_idx);
        for (size_t k = 0; k < n; ++k) V_sorted(k, i) = V(k, orig_idx);
    }
    
    return { U_sorted, S, V_sorted.transposed() };
}

} // namespace qmath
