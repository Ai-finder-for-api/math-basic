/**
 * @file UFunc.hpp
 * @brief Universal Functions (element-wise operations) and Reductions.
 * 
 * Provides vectorized math operations on NdArrays, similar to numpy.ufunc.
 * Includes trigonometric, exponential, and reduction operations.
 */
#pragma once
#include "NdArray.hpp"
#include "Broadcast.hpp"
#include <cmath>
#include <algorithm>

namespace qmath {

// ---------------------------------------------------------
// Element-wise Trigonometric Functions
// ---------------------------------------------------------
template<typename T>
NdArray<T> tan(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::tan(arr[i]);
    return result;
}

template<typename T>
NdArray<T> atan(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::atan(arr[i]);
    return result;
}

template<typename T>
NdArray<T> sinh(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::sinh(arr[i]);
    return result;
}

template<typename T>
NdArray<T> cosh(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::cosh(arr[i]);
    return result;
}

template<typename T>
NdArray<T> tanh(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::tanh(arr[i]);
    return result;
}

// ---------------------------------------------------------
// Element-wise Exponential/Logarithmic Functions
// ---------------------------------------------------------
template<typename T>
NdArray<T> log(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::log(arr[i]);
    return result;
}

template<typename T>
NdArray<T> log2(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::log2(arr[i]);
    return result;
}

template<typename T>
NdArray<T> log10(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::log10(arr[i]);
    return result;
}

template<typename T>
NdArray<T> abs(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::fabs(arr[i]);
    return result;
}

template<typename T>
NdArray<T> pow(const NdArray<T>& arr, T exponent) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::pow(arr[i], exponent);
    return result;
}

template<typename T>
NdArray<T> square(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = arr[i] * arr[i];
    return result;
}

// ---------------------------------------------------------
// Reductions (Mean, Variance, StdDev, Argmin, Argmax)
// ---------------------------------------------------------
template<typename T>
T mean(const NdArray<T>& arr) {
    if (arr.size() == 0) return 0;
    return arr.sum() / static_cast<T>(arr.size());
}

template<typename T>
T variance(const NdArray<T>& arr) {
    if (arr.size() == 0) return 0;
    T avg = mean(arr);
    T var = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        T diff = arr[i] - avg;
        var += diff * diff;
    }
    return var / static_cast<T>(arr.size());
}

template<typename T>
T std_dev(const NdArray<T>& arr) {
    return std::sqrt(variance(arr));
}

template<typename T>
size_t argmax(const NdArray<T>& arr) {
    if (arr.size() == 0) throw std::runtime_error("Empty array");
    size_t max_idx = 0;
    T max_val = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
            max_idx = i;
        }
    }
    return max_idx;
}

template<typename T>
size_t argmin(const NdArray<T>& arr) {
    if (arr.size() == 0) throw std::runtime_error("Empty array");
    size_t min_idx = 0;
    T min_val = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < min_val) {
            min_val = arr[i];
            min_idx = i;
        }
    }
    return min_idx;
}

// ---------------------------------------------------------
// Element-wise Operations with Broadcasting
// ---------------------------------------------------------
// Adds a scalar to an array
template<typename T>
NdArray<T> add_scalar(const NdArray<T>& arr, T scalar) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = arr[i] + scalar;
    return result;
}

// Element-wise addition of two arrays with broadcasting
template<typename T>
NdArray<T> add_broadcasted(const NdArray<T>& a, const NdArray<T>& b) {
    Shape out_shape = broadcast_shapes(a.shape(), b.shape());
    NdArray<T> result(out_shape);
    
    size_t total_elements = compute_size(out_shape);
    std::vector<size_t> indices(out_shape.size());
    
    for (size_t i = 0; i < total_elements; ++i) {
        // Calculate multi-dimensional index for the current flat index
        size_t temp = i;
        for (int j = out_shape.size() - 1; j >= 0; --j) {
            indices[j] = temp % out_shape[j];
            temp /= out_shape[j];
        }
        
        size_t idx_a = get_broadcasted_index(a, indices, out_shape);
        size_t idx_b = get_broadcasted_index(b, indices, out_shape);
        result[i] = a[idx_a] + b[idx_b];
    }
    return result;
}

} // namespace qmath
