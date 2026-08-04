/**
 * @file Complex.hpp
 * @brief A comprehensive C++ implementation of complex numbers.
 * 
 * This file provides a highly optimized Complex number class supporting
 * all standard arithmetic operations, trigonometric functions, exponential
 * functions, and power operations. It is heavily utilized by the quantum
 * mechanics and linear algebra modules.
 */
#pragma once
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace qmath {

template<typename T>
struct Complex {
    T real;
    T imag;

    // Constructors
    Complex() : real(0), imag(0) {}
    Complex(T r) : real(r), imag(0) {}
    Complex(T r, T i) : real(r), imag(i) {}

    // ---------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------
    Complex operator+() const { return *this; }
    Complex operator-() const { return Complex(-real, -imag); }
    
    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Complex + Complex)
    // ---------------------------------------------------------
    Complex operator+(const Complex& o) const { return Complex(real + o.real, imag + o.imag); }
    Complex operator-(const Complex& o) const { return Complex(real - o.real, imag - o.imag); }
    
    Complex operator*(const Complex& o) const {
        return Complex(
            real * o.real - imag * o.imag,
            real * o.imag + imag * o.real
        );
    }
    
    Complex operator/(const Complex& o) const {
        T denom = o.real * o.real + o.imag * o.imag;
        if (denom == 0) throw std::runtime_error("Complex division by zero");
        return Complex(
            (real * o.real + imag * o.imag) / denom,
            (imag * o.real - real * o.imag) / denom
        );
    }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Complex + Scalar)
    // ---------------------------------------------------------
    Complex operator+(T scalar) const { return Complex(real + scalar, imag); }
    Complex operator-(T scalar) const { return Complex(real - scalar, imag); }
    Complex operator*(T scalar) const { return Complex(real * scalar, imag * scalar); }
    Complex operator/(T scalar) const { 
        if (scalar == 0) throw std::runtime_error("Complex scalar division by zero");
        return Complex(real / scalar, imag / scalar); 
    }

    // ---------------------------------------------------------
    // Assignment Operators
    // ---------------------------------------------------------
    Complex& operator+=(const Complex& o) { real += o.real; imag += o.imag; return *this; }
    Complex& operator-=(const Complex& o) { real -= o.real; imag -= o.imag; return *this; }
    Complex& operator*=(const Complex& o) { *this = *this * o; return *this; }
    Complex& operator/=(const Complex& o) { *this = *this / o; return *this; }
    
    Complex& operator+=(T scalar) { real += scalar; return *this; }
    Complex& operator-=(T scalar) { real -= scalar; return *this; }
    Complex& operator*=(T scalar) { real *= scalar; imag *= scalar; return *this; }
    Complex& operator/=(T scalar) { real /= scalar; imag /= scalar; return *this; }

    // ---------------------------------------------------------
    // Comparison Operators
    // ---------------------------------------------------------
    bool operator==(const Complex& o) const { return real == o.real && imag == o.imag; }
    bool operator!=(const Complex& o) const { return !(*this == o); }

    // ---------------------------------------------------------
    // Complex Properties
    // ---------------------------------------------------------
    T magnitude_sq() const { return real * real + imag * imag; }
    T magnitude() const { return std::sqrt(magnitude_sq()); }
    T phase() const { return std::atan2(imag, real); } // Returns radians

    Complex conjugate() const { return Complex(real, -imag); }
    Complex normalize() const {
        T mag = magnitude();
        if (mag == 0) throw std::runtime_error("Cannot normalize zero complex number");
        return Complex(real / mag, imag / mag);
    }

    // ---------------------------------------------------------
    // Complex Power Functions
    // ---------------------------------------------------------
    // Euler's formula: e^(a + bi) = e^a * (cos(b) + i*sin(b))
    Complex exp() const {
        T e_a = std::exp(real);
        return Complex(e_a * std::cos(imag), e_a * std::sin(imag));
    }

    // Complex to Complex power: z^w = exp(w * log(z))
    Complex pow(const Complex& w) const {
        if (real == 0 && imag == 0) {
            if (w.real == 0 && w.imag == 0) return Complex(1, 0);
            return Complex(0, 0);
        }
        return (w * log()).exp();
    }

    // Complex to Real power
    Complex pow(T exponent) const {
        if (exponent == 0) return Complex(1, 0);
        if (real == 0 && imag == 0) return Complex(0, 0);
        
        T mag = std::pow(magnitude_sq(), exponent / 2.0);
        T ph = phase() * exponent;
        return Complex(mag * std::cos(ph), mag * std::sin(ph));
    }

    // Principal branch of the natural logarithm
    Complex log() const {
        T mag = magnitude();
        if (mag == 0) throw std::runtime_error("Logarithm of zero complex number");
        return Complex(std::log(mag), phase());
    }

    // Principal branch of the square root
    Complex sqrt() const {
        if (real == 0 && imag == 0) return Complex(0, 0);
        T mag = magnitude();
        T new_mag = std::sqrt(mag);
        T ph = phase() / 2.0;
        return Complex(new_mag * std::cos(ph), new_mag * std::sin(ph));
    }

    // ---------------------------------------------------------
    // Trigonometric Functions
    // ---------------------------------------------------------
    // sin(z) = (e^(iz) - e^(-iz)) / 2i
    Complex sin() const {
        Complex iz(-imag, real);
        Complex neg_iz(imag, -real);
        return (iz.exp() - neg_iz.exp()) / Complex(0, 2);
    }

    // cos(z) = (e^(iz) + e^(-iz)) / 2
    Complex cos() const {
        Complex iz(-imag, real);
        Complex neg_iz(imag, -real);
        return (iz.exp() + neg_iz.exp()) / 2.0;
    }

    // tan(z) = sin(z) / cos(z)
    Complex tan() const {
        Complex c = cos();
        if (c.real == 0 && c.imag == 0) throw std::runtime_error("Complex tangent undefined");
        return sin() / c;
    }

    // ---------------------------------------------------------
    // Hyperbolic Functions
    // ---------------------------------------------------------
    // sinh(z) = (e^z - e^(-z)) / 2
    Complex sinh() const {
        Complex neg_z(-real, -imag);
        return (exp() - neg_z.exp()) / 2.0;
    }

    // cosh(z) = (e^z + e^(-z)) / 2
    Complex cosh() const {
        Complex neg_z(-real, -imag);
        return (exp() + neg_z.exp()) / 2.0;
    }

    // tanh(z) = sinh(z) / cosh(z)
    Complex tanh() const {
        Complex ch = cosh();
        if (ch.real == 0 && ch.imag == 0) throw std::runtime_error("Complex tanh undefined");
        return sinh() / ch;
    }

    // ---------------------------------------------------------
    // Stream Operator for Debugging
    // ---------------------------------------------------------
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real;
        if (c.imag >= 0) os << " + " << c.imag << "i";
        else os << " - " << -c.imag << "i";
        return os;
    }
};

// Type Aliases
using ComplexF = Complex<float>;
using ComplexD = Complex<double>;

// ---------------------------------------------------------
// Free Function Overloads for Scalar + Complex
// ---------------------------------------------------------
template<typename T>
inline Complex<T> operator+(T scalar, const Complex<T>& c) { return c + scalar; }

template<typename T>
inline Complex<T> operator-(T scalar, const Complex<T>& c) { return Complex<T>(scalar - c.real, -c.imag); }

template<typename T>
inline Complex<T> operator*(T scalar, const Complex<T>& c) { return c * scalar; }

template<typename T>
inline Complex<T> operator/(T scalar, const Complex<T>& c) {
    T denom = c.real * c.real + c.imag * c.imag;
    if (denom == 0) throw std::runtime_error("Scalar / Complex division by zero");
    return Complex<T>(
        (scalar * c.real) / denom,
        (-scalar * c.imag) / denom
    );
}

} // namespace qmath
