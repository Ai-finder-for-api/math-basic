/**
 * @file CNOT.hpp
 * @brief Controlled-NOT (CNOT) Gate. Creates entanglement.
 */
#pragma once
#include "Gate.hpp"

namespace qmath {

// CNOT Gate (4x4 matrix for 2 qubits)
// |00> -> |00>, |01> -> |01>, |10> -> |11>, |11> -> |10>
template<typename T>
inline QGate<4, T> make_cnot() {
    QGate<4, T> g;
    g.set(0, 0, Complex<T>(1, 0));
    g.set(1, 1, Complex<T>(1, 0));
    g.set(2, 3, Complex<T>(1, 0)); // Flips target if control is 1
    g.set(3, 2, Complex<T>(1, 0));
    return g;
}

} // namespace qmath
