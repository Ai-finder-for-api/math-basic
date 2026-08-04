/**
 * @file Det.hpp
 * @brief Determinant calculation using LU Decomposition.
 */
#pragma once
#include "LU.hpp"
#include <stdexcept>

namespace qmath {

template<typename T>
T determinant(const DMatrix<T>& A) {
    if (A.rows() != A.cols()) throw std::runtime_error("Determinant requires a square matrix");
    
    try {
        LU_Result<T> lu = lu_decompose(A);
        T det = static_cast<T>(lu.sign);
        
        // Det(A) = Det(L) * Det(U) = 1 * Product(U_ii)
        for (size_t i = 0; i < A.rows(); ++i) {
            det *= lu.U(i, i);
        }
        return det;
    } catch (const std::runtime_error&) {
        return static_cast<T>(0); // Singular matrix has determinant 0
    }
}

} // namespace qmath
