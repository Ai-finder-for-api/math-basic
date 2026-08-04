/**
 * @file Activations.hpp
 * @brief Neural Network Activation Functions and their Derivatives.
 * 
 * Includes ReLU, Sigmoid, Tanh, Leaky ReLU, and Softmax.
 * Both forward and derivative (backward) passes are implemented.
 */
#pragma once
#include "../linalg/Matrix.hpp"
#include "../core/Constants.hpp"
#include <cmath>
#include <algorithm>

namespace qmath {

enum class ActivationType {
    RELU,
    SIGMOID,
    TANH,
    LEAKY_RELU,
    SOFTMAX,
    LINEAR
};

inline float relu(float x) { return std::fmax(0.0f, x); }
inline float relu_deriv(float x) { return x > 0.0f ? 1.0f : 0.0f; }

inline float sigmoid(float x) { return 1.0f / (1.0f + std::exp(-x)); }
inline float sigmoid_deriv(float x) { float s = sigmoid(x); return s * (1.0f - s); }

inline float tanh_fn(float x) { return std::tanh(x); }
inline float tanh_deriv(float x) { float t = std::tanh(x); return 1.0f - t * t; }

inline float leaky_relu(float x, float alpha=0.01f) { return x > 0 ? x : alpha * x; }
inline float leaky_relu_deriv(float x, float alpha=0.01f) { return x > 0 ? 1.0f : alpha; }

// Forward pass for a matrix
inline DMatrix<float> apply_activation(const DMatrix<float>& z, ActivationType type) {
    DMatrix<float> a(z.rows(), z.cols());
    for (size_t i = 0; i < z.rows(); ++i) {
        for (size_t j = 0; j < z.cols(); ++j) {
            float val = z(i, j);
            switch (type) {
                case ActivationType::RELU: a(i, j) = relu(val); break;
                case ActivationType::SIGMOID: a(i, j) = sigmoid(val); break;
                case ActivationType::TANH: a(i, j) = tanh_fn(val); break;
                case ActivationType::LEAKY_RELU: a(i, j) = leaky_relu(val); break;
                case ActivationType::LINEAR: a(i, j) = val; break;
                // Softmax is handled per-row, so handle outside or fallback to linear
                default: a(i, j) = val; break;
            }
        }
    }
    return a;
}

// Softmax applied per-row (for classification output)
inline DMatrix<float> apply_softmax(const DMatrix<float>& z) {
    DMatrix<float> a(z.rows(), z.cols());
    for (size_t i = 0; i < z.rows(); ++i) {
        float max_val = z(i, 0);
        for (size_t j = 1; j < z.cols(); ++j) max_val = std::fmax(max_val, z(i, j));
        
        float sum = 0.0f;
        for (size_t j = 0; j < z.cols(); ++j) {
            a(i, j) = std::exp(z(i, j) - max_val); // Numerical stability
            sum += a(i, j);
        }
        for (size_t j = 0; j < z.cols(); ++j) a(i, j) /= sum;
    }
    return a;
}

// Derivative pass (element-wise multiplication)
inline DMatrix<float> apply_activation_deriv(const DMatrix<float>& z, const DMatrix<float>& a, ActivationType type) {
    DMatrix<float> d(z.rows(), z.cols());
    for (size_t i = 0; i < z.rows(); ++i) {
        for (size_t j = 0; j < z.cols(); ++j) {
            float val = z(i, j);
            float a_val = a(i, j);
            switch (type) {
                case ActivationType::RELU: d(i, j) = relu_deriv(val); break;
                case ActivationType::SIGMOID: d(i, j) = a_val * (1.0f - a_val); break;
                case ActivationType::TANH: d(i, j) = 1.0f - a_val * a_val; break;
                case ActivationType::LEAKY_RELU: d(i, j) = leaky_relu_deriv(val); break;
                case ActivationType::LINEAR: d(i, j) = 1.0f; break;
                default: d(i, j) = 1.0f; break;
            }
        }
    }
    return d;
}

} // namespace qmath
