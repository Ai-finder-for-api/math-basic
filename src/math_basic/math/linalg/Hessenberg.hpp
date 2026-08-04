/**
 * @file Hessenberg.hpp
 * @brief Reduces a matrix to Hessenberg form using Householder reflections.
 * 
 * A crucial preprocessing step for the QR algorithm to find all eigenvalues.
 */
#pragma once
#include "Matrix.hpp"
#include "../core/Utils.hpp"
#include <cmath>
#include <vector>

namespace qmath {

template<typename T>
struct Hessenberg_Result {
    DMatrix<T> H;
    DMatrix<T> Q;
};

template<typename T>
Hessenberg_Result<T> hessenberg_reduce(const DMatrix<T>& A) {
    if (A.rows() != A.cols()) throw std::runtime_error("Hessenberg requires square matrix");
    size_t n = A.rows();
    
    DMatrix<T> H = A;
    DMatrix<T> Q = DMatrix<T>::identity(n);
    
    for (size_t k = 0; k < n - 2; ++k) {
        // Compute Householder vector for sub-column k
        std::vector<T> x(n - k - 1);
        T norm_x = 0;
        for (size_t i = k + 1; i < n; ++i) {
            x[i - k - 1] = H(i, k);
            norm_x += x[i - k - 1] * x[i - k - 1];
        }
        norm_x = std::sqrt(norm_x);
        
        if (norm_x < static_cast<T>(1e-12)) continue;
        
        T alpha = (H(k + 1, k) > 0) ? -norm_x : norm_x;
        
        std::vector<T> v(n - k - 1);
        v[0] = H(k + 1, k) - alpha;
        T v_norm_sq = v[0] * v[0];
        for (size_t i = 1; i < n - k - 1; ++i) {
            v[i] = x[i];
            v_norm_sq += v[i] * v[i];
        }
        
        if (v_norm_sq < static_cast<T>(1e-12)) continue;
        
        // Apply reflection from the left: H = H * (I - 2vv^T / v^Tv)
        for (size_t j = k; j < n; ++j) {
            T dot = 0;
            for (size_t i = 0; i < n - k - 1; ++i) {
                dot += H(k + 1 + i, j) * v[i];
            }
            T scale = static_cast<T>(2) * dot / v_norm_sq;
            for (size_t i = 0; i < n - k - 1; ++i) {
                H(k + 1 + i, j) -= scale * v[i];
            }
        }
        
        // Apply reflection from the right: H = (I - 2vv^T / v^Tv) * H
        for (size_t i = 0; i < n; ++i) {
            T dot = 0;
            for (size_t j = 0; j < n - k - 1; ++j) {
                dot += H(i, k + 1 + j) * v[j];
            }
            T scale = static_cast<T>(2) * dot / v_norm_sq;
            for (size_t j = 0; j < n - k - 1; ++j) {
                H(i, k + 1 + j) -= scale * v[j];
            }
        }
        
        // Update Q
        for (size_t i = 0; i < n; ++i) {
            T dot = 0;
            for (size_t j = 0; j < n - k - 1; ++j) {
                dot += Q(i, k + 1 + j) * v[j];
            }
            T scale = static_cast<T>(2) * dot / v_norm_sq;
            for (size_t j = 0; j < n - k - 1; ++j) {
                Q(i, k + 1 + j) -= scale * v[j];
            }
        }
    }
    
    return { H, Q };
}

} // namespace qmath
