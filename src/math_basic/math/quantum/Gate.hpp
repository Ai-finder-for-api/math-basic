/**
 * @file Gate.hpp
 * @brief Core engine for Quantum Gates and State Vectors.
 * 
 * Uses fixed-size N-dimensional arrays of Complex numbers for extremely
 * fast state evolution without dynamic memory allocation overhead.
 */
#pragma once
#include "../core/Complex.hpp"
#include <array>
#include <stdexcept>

namespace qmath {

// A Quantum State Vector is a 1D array of complex amplitudes of size 2^N.
template<size_t N, typename T>
struct QState {
    std::array<Complex<T>, N> amplitudes;

    QState() {
        for (auto& a : amplitudes) a = Complex<T>(0, 0);
        amplitudes[0] = Complex<T>(1, 0); // Initialize to |0...0>
    }

    // Apply a quantum gate (matrix multiplication)
    void apply(const QGate<N, T>& gate) {
        QState<N, T> new_state;
        for (size_t i = 0; i < N; ++i) {
            Complex<T> sum(0, 0);
            for (size_t j = 0; j < N; ++j) {
                sum += gate.get(i, j) * amplitudes[j];
            }
            new_state.amplitudes[i] = sum;
        }
        *this = new_state;
    }

    Complex<T> get(size_t i) const {
        if (i >= N) throw std::out_of_range("State index out of range");
        return amplitudes[i];
    }
};

// A Quantum Gate is an N x N complex matrix.
template<size_t N, typename T>
struct QGate {
    std::array<Complex<T>, N * N> data;

    QGate() {
        for (auto& d : data) d = Complex<T>(0, 0);
    }

    Complex<T>& get(size_t row, size_t col) {
        return data[row * N + col];
    }

    const Complex<T>& get(size_t row, size_t col) const {
        return data[row * N + col];
    }

    void set(size_t row, size_t col, const Complex<T>& val) {
        data[row * N + col] = val;
    }

    // Matrix Multiplication (Gate * Gate)
    QGate operator*(const QGate& o) const {
        QGate result;
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                Complex<T> sum(0, 0);
                for (size_t k = 0; k < N; ++k) {
                    sum += get(i, k) * o.get(k, j);
                }
                result.set(i, j, sum);
            }
        }
        return result;
    }

    // Apply this gate to a state vector
    void apply(QState<N, T>& state) const {
        state.apply(*this);
    }
};

} // namespace qmath
