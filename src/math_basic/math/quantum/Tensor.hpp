/**
 * @file Tensor.hpp
 * @brief Kronecker product implementation for combining quantum gates.
 * 
 * If A is m x m and B is n x n, the tensor product A ⊗ B is mn x mn.
 */
#pragma once
#include "Gate.hpp"

namespace qmath {

// Computes the Kronecker product of two quantum gates.
// Returns a new gate of size M*N.
template<size_t M, size_t N, typename T>
inline QGate<M * N, T> tensor_product(const QGate<M, T>& a, const QGate<N, T>& b) {
    QGate<M * N, T> result;
    
    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < M; ++j) {
            Complex<T> a_val = a.get(i, j);
            if (a_val.real == 0 && a_val.imag == 0) continue; // Skip zero amplitudes for speed
            
            for (size_t k = 0; k < N; ++k) {
                for (size_t l = 0; l < N; ++l) {
                    Complex<T> b_val = b.get(k, l);
                    // The new row/col indices are calculated by block
                    size_t row = i * N + k;
                    size_t col = j * N + l;
                    result.set(row, col, a_val * b_val);
                }
            }
        }
    }
    return result;
}

} // namespace qmath
