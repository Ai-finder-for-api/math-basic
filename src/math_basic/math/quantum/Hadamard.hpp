/**
 * @file Hadamard.hpp
 * @brief Hadamard (H) Gate. Creates superposition.
 */
#pragma once
#include "Gate.hpp"
#include "../core/Constants.hpp"

namespace qmath {

// Hadamard Gate
// |0> -> (|0> + |1>) / sqrt(2)
// |1> -> (|0> - |1>) / sqrt(2)
template<typename T>
inline QGate<2, T> make_hadamard() {
    T inv_sqrt2 = static_cast<T>(0.70710678118654752440); // 1 / sqrt(2)
    QGate<2, T> g;
    g.set(0, 0, Complex<T>(inv_sqrt2, 0));
    g.set(0, 1, Complex<T>(inv_sqrt2, 0));
    g.set(1, 0, Complex<T>(inv_sqrt2, 0));
    g.set(1, 1, Complex<T>(-inv_sqrt2, 0));
    return g;
}

} // namespace qmath
