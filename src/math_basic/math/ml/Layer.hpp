/**
 * @file Layer.hpp
 * @brief A Dense (Fully Connected) Neural Network Layer.
 * 
 * Implements forward propagation (z = Wx + b) and backward propagation
 * (computing gradients for weights and biases).
 */
#pragma once
#include "Activations.hpp"
#include "../linalg/Matrix.hpp"
#include <vector>
#include <random>

namespace qmath {

class DenseLayer {
public:
    DMatrix<float> weights; // Shape: (out_features, in_features)
    DMatrix<float> biases;  // Shape: (out_features, 1)
    DMatrix<float> z;       // Pre-activation
    DMatrix<float> a;       // Post-activation
    DMatrix<float> grad_W;  // Weight gradients
    DMatrix<float> grad_b;  // Bias gradients
    ActivationType activation;

    DenseLayer(size_t in_features, size_t out_features, ActivationType act) 
        : weights(out_features, in_features), biases(out_features, 1), 
          activation(act) 
    {
        // He initialization for weights
        std::random_device rd;
        std::mt19937 gen(rd());
        float std_dev = std::sqrt(2.0f / in_features);
        std::normal_distribution<float> d(0.0f, std_dev);
        
        for (size_t i = 0; i < weights.rows(); ++i) {
            for (size_t j = 0; j < weights.cols(); ++j) {
                weights(i, j) = d(gen);
            }
        }
        for (size_t i = 0; i < biases.rows(); ++i) biases(i, 0) = 0.0f;
    }

    // Forward pass: input shape (in_features, batch_size)
    DMatrix<float> forward(const DMatrix<float>& input) {
        z = weights * input + biases; // Broadcast biases (simplified for batch_size=1 conceptually, or requires explicit addition)
        // For simplicity, assuming biases are added per column. Since DMatrix doesn't support broadcasting yet natively,
        // we manually add biases to each column.
        for (size_t j = 0; j < z.cols(); ++j) {
            for (size_t i = 0; i < z.rows(); ++i) {
                z(i, j) += biases(i, 0);
            }
        }
        
        if (activation == ActivationType::SOFTMAX) {
            a = apply_softmax(z);
        } else {
            a = apply_activation(z, activation);
        }
        return a;
    }

    // Backward pass: da is gradient from next layer
    // input is the activation from the previous layer
    DMatrix<float> backward(const DMatrix<float>& da, const DMatrix<float>& input) {
        DMatrix<float> dz;
        if (activation == ActivationType::SOFTMAX) {
            // Softmax derivative is complex with cross-entropy, usually dz = a - y.
            // Assuming da is actually dz for softmax+ce combo.
            dz = da;
        } else {
            DMatrix<float> deriv = apply_activation_deriv(z, a, activation);
            dz = DMatrix<float>(da.rows(), da.cols());
            for (size_t i = 0; i < da.rows(); ++i) {
                for (size_t j = 0; j < da.cols(); ++j) {
                    dz(i, j) = da(i, j) * deriv(i, j);
                }
            }
        }
        
        // Gradients: grad_W = dz * input^T, grad_b = sum(dz, axis=1)
        DMatrix<float> input_T = input.transposed();
        grad_W = dz * input_T;
        
        grad_b = DMatrix<float>(biases.rows(), 1);
        for (size_t i = 0; i < dz.rows(); ++i) {
            float sum = 0.0f;
            for (size_t j = 0; j < dz.cols(); ++j) sum += dz(i, j);
            grad_b(i, 0) = sum / static_cast<float>(dz.cols());
        }
        
        // Propagate gradient backwards: da_prev = W^T * dz
        DMatrix<float> w_T = weights.transposed();
        return w_T * dz;
    }
};

} // namespace qmath
