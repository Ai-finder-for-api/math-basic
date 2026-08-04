#pragma once
#include "Simd.hpp"
#include "Allocator.hpp"
#include <vector>
#include <cstring>
#include <stdexcept>
#include <initializer_list>
#include <numeric>
#include <functional>
#include <algorithm>
#include <cmath>
#include <type_traits>

namespace qmath {

using Shape = std::vector<size_t>;
using Strides = std::vector<size_t>;

inline size_t compute_size(const Shape& shape) {
    if (shape.empty()) return 0;
    size_t size = 1;
    for (size_t s : shape) size *= s;
    return size;
}

inline Strides compute_strides(const Shape& shape, size_t itemsize) {
    Strides strides(shape.size());
    if (shape.empty()) return strides;
    strides[shape.size() - 1] = itemsize;
    for (int i = shape.size() - 2; i >= 0; --i) {
        strides[i] = strides[i + 1] * shape[i + 1];
    }
    return strides;
}

template <typename T>
class NdArray {
private:
    Shape shape_;
    Strides strides_;
    std::vector<T, AlignedAllocator<T>> data_;

public:
    NdArray() = default;
    NdArray(const Shape& shape) : shape_(shape), strides_(compute_strides(shape, sizeof(T))), data_(compute_size(shape)) {}
    
    NdArray(std::initializer_list<T> list) : shape_({list.size()}), strides_(compute_strides({list.size()}, sizeof(T))), data_(list) {}
    
    NdArray(const std::vector<T>& vec) : shape_({vec.size()}), strides_(compute_strides({vec.size()}, sizeof(T))), data_(vec.begin(), vec.end()) {}

    NdArray(const Shape& shape, const std::vector<T>& data) : shape_(shape), strides_(compute_strides(shape, sizeof(T))), data_(data.begin(), data.end()) {
        if (data_.size() != compute_size(shape)) throw std::runtime_error("Data size mismatch");
    }

    const Shape& shape() const { return shape_; }
    const Strides& strides() const { return strides_; }
    size_t ndim() const { return shape_.size(); }
    size_t size() const { return data_.size(); }
    T* data() { return data_.data(); }
    const T* data() const { return data_.data(); }

    T& operator[](size_t i) { return data_[i]; }
    const T& operator[](size_t i) const { return data_[i]; }

    size_t flat_index(const std::vector<size_t>& indices) const {
        if (indices.size() != ndim()) throw std::runtime_error("Dimension mismatch");
        size_t idx = 0;
        for (size_t i = 0; i < ndim(); ++i) {
            if (indices[i] >= shape_[i]) throw std::runtime_error("Index out of bounds");
            idx += indices[i] * strides_[i];
        }
        return idx / sizeof(T);
    }

    T& at(const std::vector<size_t>& indices) { return data_[flat_index(indices)]; }
    const T& at(const std::vector<size_t>& indices) const { return data_[flat_index(indices)]; }

    NdArray reshape(const Shape& new_shape) const {
        if (compute_size(new_shape) != size()) throw std::runtime_error("Cannot reshape size mismatch");
        NdArray result(new_shape);
        result.data_ = this->data_;
        return result;
    }

    NdArray transpose() const {
        if (ndim() != 2) throw std::runtime_error("Transpose currently supports 2D only");
        Shape new_shape = {shape_[1], shape_[0]};
        NdArray result(new_shape);
        for (size_t i = 0; i < shape_[0]; ++i) {
            for (size_t j = 0; j < shape_[1]; ++j) {
                result.at({j, i}) = this->at({i, j});
            }
        }
        return result;
    }

    // SIMD Accelerated addition (x86 only, fallback for ARM)
    NdArray operator+(const NdArray& other) const {
        if (shape_ != other.shape_) throw std::runtime_error("Shape mismatch for element-wise add");
        NdArray result(shape_);
        
        #if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
        if constexpr (std::is_same_v<T, float>) {
            size_t i = 0;
            for (; i + 4 <= size(); i += 4) {
                __m128 a = _mm_load_ps(&data_[i]);
                __m128 b = _mm_load_ps(&other.data_[i]);
                _mm_store_ps(&result.data_[i], _mm_add_ps(a, b));
            }
            for (; i < size(); ++i) result.data_[i] = data_[i] + other.data_[i];
        } else {
            for (size_t i = 0; i < size(); ++i) result.data_[i] = data_[i] + other.data_[i];
        }
        #else
        for (size_t i = 0; i < size(); ++i) result.data_[i] = data_[i] + other.data_[i];
        #endif
        return result;
    }

    NdArray operator*(const NdArray& other) const {
        if (shape_ != other.shape_) throw std::runtime_error("Shape mismatch for element-wise mul");
        NdArray result(shape_);
        
        #if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
        if constexpr (std::is_same_v<T, float>) {
            size_t i = 0;
            for (; i + 4 <= size(); i += 4) {
                __m128 a = _mm_load_ps(&data_[i]);
                __m128 b = _mm_load_ps(&other.data_[i]);
                _mm_store_ps(&result.data_[i], _mm_mul_ps(a, b));
            }
            for (; i < size(); ++i) result.data_[i] = data_[i] * other.data_[i];
        } else {
            for (size_t i = 0; i < size(); ++i) result.data_[i] = data_[i] * other.data_[i];
        }
        #else
        for (size_t i = 0; i < size(); ++i) result.data_[i] = data_[i] * other.data_[i];
        #endif
        return result;
    }

    NdArray operator*(T scalar) const {
        NdArray result(shape_);
        for (size_t i = 0; i < size(); ++i) result.data_[i] = data_[i] * scalar;
        return result;
    }

    friend NdArray operator*(T scalar, const NdArray& arr) {
        return arr * scalar;
    }

    NdArray operator+(T scalar) const {
        NdArray result(shape_);
        #if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
        if constexpr (std::is_same_v<T, float>) {
            __m128 s = _mm_set1_ps(scalar);
            size_t i = 0;
            for (; i + 4 <= size(); i += 4) {
                _mm_store_ps(&result.data_[i], _mm_add_ps(_mm_load_ps(&data_[i]), s));
            }
            for (; i < size(); ++i) result.data_[i] = data_[i] + scalar;
        } else {
            for (size_t i = 0; i < size(); ++i) result.data_[i] = data_[i] + scalar;
        }
        #else
        for (size_t i = 0; i < size(); ++i) result.data_[i] = data_[i] + scalar;
        #endif
        return result;
    }

    NdArray matmul(const NdArray& other) const {
        if (ndim() != 2 || other.ndim() != 2 || shape_[1] != other.shape_[0])
            throw std::runtime_error("Matmul dimension mismatch");
        
        Shape new_shape = {shape_[0], other.shape_[1]};
        NdArray result(new_shape);
        
        for (size_t i = 0; i < new_shape[0]; ++i) {
            for (size_t j = 0; j < new_shape[1]; ++j) {
                T sum = 0;
                for (size_t k = 0; k < shape_[1]; ++k) {
                    sum += at({i, k}) * other.at({k, j});
                }
                result.at({i, j}) = sum;
            }
        }
        return result;
    }

    T sum() const {
        T s = 0;
        for (size_t i = 0; i < size(); ++i) s += data_[i];
        return s;
    }

    T max() const {
        if (size() == 0) throw std::runtime_error("Empty array");
        T m = data_[0];
        for (size_t i = 1; i < size(); ++i) if (data_[i] > m) m = data_[i];
        return m;
    }

    T min() const {
        if (size() == 0) throw std::runtime_error("Empty array");
        T m = data_[0];
        for (size_t i = 1; i < size(); ++i) if (data_[i] < m) m = data_[i];
        return m;
    }

    py::buffer_info get_buffer_info() {
        return py::buffer_info(
            data_.data(), sizeof(T), py::format_descriptor<T>::format(),
            ndim(), shape_, strides_
        );
    }
};

template<typename T>
NdArray<T> sin(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::sin(arr[i]);
    return result;
}

template<typename T>
NdArray<T> cos(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::cos(arr[i]);
    return result;
}

template<typename T>
NdArray<T> exp(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::exp(arr[i]);
    return result;
}

template<typename T>
NdArray<T> sqrt(const NdArray<T>& arr) {
    NdArray<T> result(arr.shape());
    for (size_t i = 0; i < arr.size(); ++i) result[i] = std::sqrt(arr[i]);
    return result;
}

} // namespace qmath
