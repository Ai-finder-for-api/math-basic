/**
 * @file Norm.hpp
 * @brief Vector and Matrix Norms.
 */
#pragma once
#include "Matrix.hpp"
#include "../3d/Vec3.hpp"
#include "../3d/Vec4.hpp"
#include <cmath>

namespace qmath {

// Vector Norms
template<typename T>
inline T norm_l1(const Vec3<T>& v) {
    return std::fabs(v.x) + std::fabs(v.y) + std::fabs(v.z);
}

template<typename T>
inline T norm_l2(const Vec3<T>& v) {
    return v.length();
}

template<typename T>
inline T norm_linf(const Vec3<T>& v) {
    return std::fmax(std::fabs(v.x), std::fmax(std::fabs(v.y), std::fabs(v.z)));
}

// Matrix Frobenius Norm
template<typename T>
inline T norm_frobenius(const DMatrix<T>& A) {
    T sum = static_cast<T>(0);
    for (size_t i = 0; i < A.rows(); ++i) {
        for (size_t j = 0; j < A.cols(); ++j) {
            sum += A(i, j) * A(i, j);
        }
    }
    return std::sqrt(sum);
}

// Matrix 1-Norm (maximum absolute column sum)
template<typename T>
inline T norm_1(const DMatrix<T>& A) {
    T max_sum = static_cast<T>(0);
    for (size_t j = 0; j < A.cols(); ++j) {
        T col_sum = static_cast<T>(0);
        for (size_t i = 0; i < A.rows(); ++i) {
            col_sum += std::fabs(A(i, j));
        }
        if (col_sum > max_sum) max_sum = col_sum;
    }
    return max_sum;
}

// Matrix Infinity-Norm (maximum absolute row sum)
template<typename T>
inline T norm_inf(const DMatrix<T>& A) {
    T max_sum = static_cast<T>(0);
    for (size_t i = 0; i < A.rows(); ++i) {
        T row_sum = static_cast<T>(0);
        for (size_t j = 0; j < A.cols(); ++j) {
            row_sum += std::fabs(A(i, j));
        }
        if (row_sum > max_sum) max_sum = row_sum;
    }
    return max_sum;
}

} // namespace qmath
