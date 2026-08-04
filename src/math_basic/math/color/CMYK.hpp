/**
 * @file CMYK.hpp
 * @brief CMYK (Cyan, Magenta, Yellow, Key/Black) Color Space.
 */
#pragma once
#include "RGB.hpp"
#include "../core/Utils.hpp"
#include <algorithm>

namespace qmath {

template<typename T>
struct ColorCMYK {
    T c, m, y, k;

    ColorCMYK(T c, T m, T y, T k) : c(c), m(m), y(y), k(k) {}

    static ColorCMYK from_rgb(const ColorRGB<T>& rgb) {
        T k = static_cast<T>(1) - rgb.max_component();
        if (k >= static_cast<T>(1)) return ColorCMYK(0, 0, 0, 1);
        
        T c = (static_cast<T>(1) - rgb.r - k) / (static_cast<T>(1) - k);
        T m = (static_cast<T>(1) - rgb.g - k) / (static_cast<T>(1) - k);
        T y = (static_cast<T>(1) - rgb.b - k) / (static_cast<T>(1) - k);
        
        return ColorCMYK(c, m, y, k);
    }

    ColorRGB<T> to_rgb() const {
        T r = static_cast<T>(1) - (c * (static_cast<T>(1) - k) + k);
        T g = static_cast<T>(1) - (m * (static_cast<T>(1) - k) + k);
        T b = static_cast<T>(1) - (y * (static_cast<T>(1) - k) + k);
        return ColorRGB<T>(r, g, b);
    }
};

} // namespace qmath
