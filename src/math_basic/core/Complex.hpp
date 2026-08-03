#pragma once

struct Complex {
    float real;
    float imag;
    
    Complex(float r = 0.0f, float i = 0.0f) : real(r), imag(i) {}
    
    Complex operator*(const Complex& o) const {
        return Complex(real * o.real - imag * o.imag, real * o.imag + imag * o.real);
    }
    Complex operator+(const Complex& o) const {
        return Complex(real + o.real, imag + o.imag);
    }
};
