#pragma once
#include "Matrix.hpp"
#include <stdexcept>

namespace qmath {
template<typename T>
inline T trace(const DMatrix<T>& A) {
    if (A.rows() != A.cols()) throw std::runtime_error("Trace requires square matrix");
    T tr = static_cast<T>(0);
    for (size_t i = 0; i < A.rows(); ++i) tr += A(i, i);
    return tr;
}
} // namespace qmath
