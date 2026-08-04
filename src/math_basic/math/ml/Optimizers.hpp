/**
 * @file Optimizers.hpp
 * @brief Gradient Descent Optimizers (SGD, Adam).
 */
#pragma once
#include "Layer.hpp"
#include <cmath>

namespace qmath {

class AdamOptimizer {
private:
    float learning_rate;
    float beta1;
    float beta2;
    float epsilon;
    int t; // Timestep
    
    // Moment vectors for each layer's weights and biases
    std::vector<DMatrix<float>> m_W;
    std::vector<DMatrix<float>> v_W;
    std::vector<DMatrix<float>> m_b;
    std::vector<DMatrix<float>> v_b;

public:
    AdamOptimizer(float lr = 0.001f, float b1 = 0.9f, float b2 = 0.999f, float eps = 1e-8f)
        : learning_rate(lr), beta1(b1), beta2(b2), epsilon(eps), t(0) {}

    void initialize(const std::vector<DenseLayer>& layers) {
        m_W.clear(); v_W.clear(); m_b.clear(); v_b.clear();
        for (const auto& layer : layers) {
            m_W.push_back(DMatrix<float>(layer.weights.rows(), layer.weights.cols()));
            v_W.push_back(DMatrix<float>(layer.weights.rows(), layer.weights.cols()));
            m_b.push_back(DMatrix<float>(layer.biases.rows(), layer.biases.cols()));
            v_b.push_back(DMatrix<float>(layer.biases.rows(), layer.biases.cols()));
        }
    }

    void step(std::vector<DenseLayer>& layers) {
        t++;
        float bias_correction1 = 1.0f - std::pow(beta1, t);
        float bias_correction2 = 1.0f - std::pow(beta2, t);

        for (size_t l = 0; l < layers.size(); ++l) {
            // Update Weights
            for (size_t i = 0; i < layers[l].weights.rows(); ++i) {
                for (size_t j = 0; j < layers[l].weights.cols(); ++j) {
                    float grad = layers[l].grad_W(i, j);
                    m_W[l](i, j) = beta1 * m_W[l](i, j) + (1.0f - beta1) * grad;
                    v_W[l](i, j) = beta2 * v_W[l](i, j) + (1.0f - beta2) * grad * grad;
                    
                    float m_hat = m_W[l](i, j) / bias_correction1;
                    float v_hat = v_W[l](i, j) / bias_correction2;
                    
                    layers[l].weights(i, j) -= learning_rate * m_hat / (std::sqrt(v_hat) + epsilon);
                }
            }
            
            // Update Biases
            for (size_t i = 0; i < layers[l].biases.rows(); ++i) {
                for (size_t j = 0; j < layers[l].biases.cols(); ++j) {
                    float grad = layers[l].grad_b(i, j);
                    m_b[l](i, j) = beta1 * m_b[l](i, j) + (1.0f - beta1) * grad;
                    v_b[l](i, j) = beta2 * v_b[l](i, j) + (1.0f - beta2) * grad * grad;
                    
                    float m_hat = m_b[l](i, j) / bias_correction1;
                    float v_hat = v_b[l](i, j) / bias_correction2;
                    
                    layers[l].biases(i, j) -= learning_rate * m_hat / (std::sqrt(v_hat) + epsilon);
                }
            }
        }
    }
};

} // namespace qmath
