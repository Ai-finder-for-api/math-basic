/**
 * @file Lab.hpp
 * @brief CIELAB Color Space (perceptually uniform).
 */
#pragma once
#include "RGB.hpp"
#include "XYZ.hpp"
#include "../core/Constants.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct ColorLab {
    T l, a, b;

    ColorLab(T l, T a, T b) : l(l), a(a), b(b) {}

    static ColorLab from_xyz(const ColorXYZ<T>& xyz) {
        T x = xyz.x / static_cast<T>(95.047);
        T y = xyz.y / static_cast<T>(100.0);
        T z = xyz.z / static_cast<T>(108.883);

        auto f = [](T t) {
            T d = static_cast<T>(6.0/29.0);
            if (t > d * d * d) return std::cbrt(t);
            return t / (static_cast<T>(3) * d * d) + static_cast<T>(4.0/29.0);
        };

        T fx = f(x);
        T fy = f(y);
        T fz = f(z);

        T l = static_cast<T>(116) * fy - static_cast<T>(16);
        T a = static_cast<T>(500) * (fx - fy);
        T b = static_cast<T>(200) * (fy - fz);

        return ColorLab(l, a, b);
    }

    ColorXYZ<T> to_xyz() const {
        T fy = (l + static_cast<T>(16)) / static_cast<T>(116);
        T fx = a / static_cast<T>(500) + fy;
        T fz = fy - b / static_cast<T>(200);

        auto f_inv = [](T t) {
            T d = static_cast<T>(6.0/29.0);
            if (t > d) return t * t * t;
            return static_cast<T>(3) * d * d * (t - static_cast<T>(4.0/29.0));
        };

        T x = f_inv(fx) * static_cast<T>(95.047);
        T y = f_inv(fy) * static_cast<T>(100.0);
        T z = f_inv(fz) * static_cast<T>(108.883);

        return ColorXYZ<T>(x, y, z);
    }
};

} // namespace qmath
