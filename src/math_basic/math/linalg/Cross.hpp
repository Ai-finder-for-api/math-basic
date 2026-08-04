#pragma once
#include "../3d/Vec3.hpp"

namespace qmath {
template<typename T>
inline Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b) { return a.cross(b); }
} // namespace qmath
