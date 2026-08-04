#pragma once
#include "Dot.hpp"

namespace qmath {
template<typename T>
inline T inner(const std::vector<T>& a, const std::vector<T>& b) { return dot(a, b); }
} // namespace qmath
