#pragma once
#include "core/Complex.hpp"
#include <array>
#include <stdexcept>

namespace qmath {

// Define QGate FIRST so QState can see it!
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
};

template<size_t N, typename T>
struct QState {
    std::array<Complex<T>, N> amplitudes;

    QState() {
        for (auto& a : amplitudes) a = Complex<T>(0, 0);
        amplitudes[0] = Complex<T>(1, 0);
    }

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

} // namespace qmath
