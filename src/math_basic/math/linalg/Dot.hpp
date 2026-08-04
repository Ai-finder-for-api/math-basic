#pragma once
#include "../3d/Vec3.hpp"
#include "../3d/Vec4.hpp"
#include "../3d/Vec2.hpp"
#include <vector>

namespace qmath {

template<typename T>
inline T dot(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) throw std::runtime_error("Vector size mismatch");
    T sum = 0;
    for (size_t i = 0; i < a.size(); ++i) sum += a[i] * b[i];
    return sum;
}

template<typename T>
inline T dot(const Vec2<T>& a, const Vec2<T>& b) { return a.dot(b); }
template<typename T>
inline T dot(const Vec3<T>& a, const Vec3<T>& b) { return a.dot(b); }
template<typename T>
inline T dot(const Vec4<T>& a, const Vec4<T>& b) { return a.dot(b); }

} // namespace qmath
