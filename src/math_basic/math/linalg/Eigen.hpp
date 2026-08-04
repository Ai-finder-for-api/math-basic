/**
 * @file Eigen.hpp
 * @brief Eigenvalue and Eigenvector computation using the Jacobi method.
 * 
 * A robust iterative algorithm for finding eigenvalues of symmetric matrices.
 * Crucial for Principal Component Analysis (PCA) and physics simulations.
 */
#pragma once
#include <utility>
#include "Matrix.hpp"
#include "../core/Utils.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

namespace qmath {

template<typename T>
struct Eigen_Result {
    std::vector<T> eigenvalues;
    DMatrix<T> eigenvectors; // Columns are eigenvectors
};

template<typename T>
Eigen_Result<T> jacobi_eigen(const DMatrix<T>& A, size_t max_iter = 100, T tolerance = static_cast<T>(1e-8)) {
    if (A.rows() != A.cols()) throw std::runtime_error("Eigenvalue computation requires square matrix");
    size_t n = A.rows();
    
    DMatrix<T> a = A; // Copy to modify
    DMatrix<T> v(n, n); // Eigenvector matrix
    
    // Initialize v to identity
    for (size_t i = 0; i < n; ++i) v(i, i) = static_cast<T>(1);

    for (size_t iter = 0; iter < max_iter; ++iter) {
        // Find largest off-diagonal element
        size_t p = 0, q = 1;
        T max_val = static_cast<T>(0);
        for (size_t i = 0; i < n - 1; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                T val = std::fabs(a(i, j));
                if (val > max_val) {
                    max_val = val;
                    p = i;
                    q = j;
                }
            }
        }

        // Check for convergence
        if (max_val < tolerance) break;

        // Compute rotation angle
        T theta;
        if (std::fabs(a(p, p) - a(q, q)) < static_cast<T>(1e-20)) {
            theta = static_cast<T>(PI_D / 4.0);
        } else {
            theta = static_cast<T>(0.5) * std::atan2(static_cast<T>(2) * a(p, q), a(q, q) - a(p, p));
        }
        
        T c = std::cos(theta);
        T s = std::sin(theta);

        // Apply rotation to A
        for (size_t i = 0; i < n; ++i) {
            if (i != p && i != q) {
                T aip = a(i, p);
                T aiq = a(i, q);
                a(i, p) = c * aip - s * aiq;
                a(p, i) = a(i, p);
                a(i, q) = s * aip + c * aiq;
                a(q, i) = a(i, q);
            }
        }
        
        T app = a(p, p);
        T aqq = a(q, q);
        T apq = a(p, q);

        a(p, p) = c * c * app - static_cast<T>(2) * s * c * apq + s * s * aqq;
        a(q, q) = s * s * app + static_cast<T>(2) * s * c * apq + c * c * aqq;
        a(p, q) = static_cast<T>(0);
        a(q, p) = static_cast<T>(0);

        // Update eigenvector matrix
        for (size_t i = 0; i < n; ++i) {
            T vip = v(i, p);
            T viq = v(i, q);
            v(i, p) = c * vip - s * viq;
            v(i, q) = s * vip + c * viq;
        }
    }

    // Extract eigenvalues
    std::vector<T> eigenvalues(n);
    for (size_t i = 0; i < n; ++i) eigenvalues[i] = a(i, i);

    return { eigenvalues, v };
}

} // namespace qmath
