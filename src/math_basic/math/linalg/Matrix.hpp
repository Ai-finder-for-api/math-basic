/**
 * @file Matrix.hpp
 * @brief Dynamic 2D Matrix implementation for Linear Algebra operations.
 * 
 * Unlike the fixed-size 3D math matrices, this is designed for arbitrary
 * N x M matrices, similar to NumPy's 2D arrays.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Allocator.hpp"
#include <vector>
#include <stdexcept>
#include <cmath>

namespace qmath {

template<typename T>
class DMatrix {
private:
    size_t rows_, cols_;
    std::vector<T, AlignedAllocator<T>> data_;

public:
    DMatrix() : rows_(0), cols_(0) {}
    DMatrix(size_t rows, size_t cols) : rows_(rows), cols_(cols), data_(rows * cols, 0) {}
    
    DMatrix(size_t rows, size_t cols, const std::vector<T>& data) : rows_(rows), cols_(cols), data_(data.begin(), data.end()) {
        if (data_.size() != rows * cols) throw std::runtime_error("Data size mismatch");
    }

    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    T* data() { return data_.data(); }
    const T* data() const { return data_.data(); }

    T& operator()(size_t r, size_t c) {
        if (r >= rows_ || c >= cols_) throw std::out_of_range("Matrix index out of bounds");
        return data_[r * cols_ + c];
    }

    const T& operator()(size_t r, size_t c) const {
        if (r >= rows_ || c >= cols_) throw std::out_of_range("Matrix index out of bounds");
        return data_[r * cols_ + c];
    }

    DMatrix transposed() const {
        DMatrix t(cols_, rows_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < cols_; ++j) {
                t(j, i) = (*this)(i, j);
            }
        }
        return t;
    }

    DMatrix operator*(const DMatrix& o) const {
        if (cols_ != o.rows_) throw std::runtime_error("Matrix dimension mismatch for multiplication");
        DMatrix res(rows_, o.cols_);
        for (size_t i = 0; i < rows_; ++i) {
            for (size_t j = 0; j < o.cols_; ++j) {
                T sum = 0;
                for (size_t k = 0; k < cols_; ++k) {
                    sum += (*this)(i, k) * o(k, j);
                }
                res(i, j) = sum;
            }
        }
        return res;
    }
};

} // namespace qmath
