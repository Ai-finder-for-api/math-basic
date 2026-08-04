/**
 * @file QR.hpp
 * @brief QR Decomposition using Householder reflections.
 * 
 * Decomposes a matrix A into an orthogonal matrix Q and an upper triangular
 * matrix R such that A = Q * R.
 */
#pragma once
#include "Matrix.hpp"
#include "../core/Utils.hpp"
#include <cmath>
#include <vector>

namespace qmath {

template<typename T>
struct QR_Result {
    DMatrix<T> Q;
    DMatrix<T> R;
};

template<typename T>
QR_Result<T> qr_decompose(const DMatrix<T>& A) {
    size_t m = A.rows();
    size_t n = A.cols();
    
    DMatrix<T> Q(m, m);
    DMatrix<T> R = A;
    
    // Initialize Q to identity
    for (size_t i = 0; i < m; ++i) Q(i, i) = static_cast<T>(1);
    
    size_t k_max = std::min(m - 1, n);
    
    for (size_t k = 0; k < k_max; ++k) {
        // Compute Householder vector for column k
        std::vector<T> x(m - k);
        T norm_x = static_cast<T>(0);
        for (size_t i = k; i < m; ++i) {
            x[i - k] = R(i, k);
            norm_x += x[i - k] * x[i - k];
        }
        norm_x = std::sqrt(norm_x);
        
        if (norm_x < static_cast<T>(1e-12)) continue;
        
        T alpha = (R(k, k) > 0) ? -norm_x : norm_x;
        
        std::vector<T> v(m - k);
        v[0] = R(k, k) - alpha;
        T v_norm_sq = v[0] * v[0];
        for (size_t i = 1; i < m - k; ++i) {
            v[i] = x[i];
            v_norm_sq += v[i] * v[i];
        }
        
        if (v_norm_sq < static_cast<T>(1e-12)) continue;
        
        // Apply reflection to R
        for (size_t j = k; j < n; ++j) {
            T dot = static_cast<T>(0);
            for (size_t i = k; i < m; ++i) {
                dot += R(i, j) * v[i - k];
            }
            T scale = static_cast<T>(2) * dot / v_norm_sq;
            for (size_t i = k; i < m; ++i) {
                R(i, j) -= scale * v[i - k];
            }
        }
        
        // Apply reflection to Q
        for (size_t j = 0; j < m; ++j) {
            T dot = static_cast<T>(0);
            for (size_t i = k; i < m; ++i) {
                dot += Q(j, i) * v[i - k];
            }
            T scale = static_cast<T>(2) * dot / v_norm_sq;
            for (size_t i = k; i < m; ++i) {
                Q(j, i) -= scale * v[i - k];
            }
        }
    }
    
    return { Q, R };
}

} // namespace qmath
