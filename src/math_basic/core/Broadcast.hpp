/**
 * @file Broadcast.hpp
 * @brief NumPy-style shape broadcasting logic.
 * 
 * Determines the output shape when performing element-wise operations
 * on arrays of different shapes (e.g., adding a 1D array to a 2D array).
 */
#pragma once
#include "NdArray.hpp"
#include <stdexcept>

namespace qmath {

// Compute the broadcasted shape from two input shapes
inline Shape broadcast_shapes(const Shape& s1, const Shape& s2) {
    size_t n = std::max(s1.size(), s2.size());
    Shape result(n);
    
    for (size_t i = 0; i < n; ++i) {
        size_t dim1 = (i < n - s1.size()) ? 1 : s1[i - (n - s1.size())];
        size_t dim2 = (i < n - s2.size()) ? 1 : s2[i - (n - s2.size())];
        
        if (dim1 == dim2) {
            result[i] = dim1;
        } else if (dim1 == 1) {
            result[i] = dim2;
        } else if (dim2 == 1) {
            result[i] = dim1;
        } else {
            throw std::runtime_error("Shapes cannot be broadcast together");
        }
    }
    return result;
}

// Helper to compute the flat index for a broadcasted array
template<typename T>
inline size_t get_broadcasted_index(const NdArray<T>& arr, const std::vector<size_t>& indices, const Shape& out_shape) {
    size_t idx = 0;
    size_t offset = out_shape.size() - arr.ndim();
    
    for (size_t i = 0; i < arr.ndim(); ++i) {
        size_t dim_size = arr.shape()[i];
        size_t actual_idx = (dim_size == 1) ? 0 : indices[i + offset];
        idx += actual_idx * arr.strides()[i];
    }
    return idx / sizeof(T);
}

} // namespace qmath
