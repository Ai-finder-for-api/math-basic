#pragma once
#include "Matrix.hpp"
#include <vector>

namespace qmath {
template<typename T>
inline DMatrix<T> outer(const std::vector<T>& a, const std::vector<T>& b) {
    DMatrix<T> result(a.size(), b.size());
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b.size(); ++j) {
            result(i, j) = a[i] * b[j];
        }
    }
    return result;
}
} // namespace qmath
