/**
 * @file Pauli.hpp
 * @brief Standard Pauli Quantum Gates (X, Y, Z).
 */
#pragma once
#include "Gate.hpp"

namespace qmath {

// Pauli-X Gate (Quantum NOT Gate)
// |0> -> |1>, |1> -> |0>
template<typename T>
inline QGate<2, T> make_pauli_x() {
    QGate<2, T> g;
    g.set(0, 1, Complex<T>(1, 0));
    g.set(1, 0, Complex<T>(1, 0));
    return g;
}

// Pauli-Y Gate
// |0> -> i|1>, |1> -> -i|0>
template<typename T>
inline QGate<2, T> make_pauli_y() {
    QGate<2, T> g;
    g.set(0, 1, Complex<T>(0, -1));
    g.set(1, 0, Complex<T>(0, 1));
    return g;
}

// Pauli-Z Gate
// |0> -> |0>, |1> -> -|1>
template<typename T>
inline QGate<2, T> make_pauli_z() {
    QGate<2, T> g;
    g.set(0, 0, Complex<T>(1, 0));
    g.set(1, 1, Complex<T>(-1, 0));
    return g;
}

} // namespace qmath
