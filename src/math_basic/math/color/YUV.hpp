/**
 * @file YUV.hpp
 * @brief YUV Color Space (used in video compression).
 */
#pragma once
#include "RGB.hpp"

namespace qmath {

template<typename T>
struct ColorYUV {
    T y, u, v;

    ColorYUV(T y, T u, T v) : y(y), u(u), v(v) {}

    static ColorYUV from_rgb(const ColorRGB<T>& rgb) {
        T y = static_cast<T>(0.299) * rgb.r + static_cast<T>(0.587) * rgb.g + static_cast<T>(0.114) * rgb.b;
        T u = static_cast<T>(-0.14713) * rgb.r - static_cast<T>(0.28886) * rgb.g + static_cast<T>(0.436) * rgb.b;
        T v = static_cast<T>(0.615) * rgb.r - static_cast<T>(0.51499) * rgb.g - static_cast<T>(0.10001) * rgb.b;
        return ColorYUV(y, u, v);
    }

    ColorRGB<T> to_rgb() const {
        T r = y + static_cast<T>(1.13983) * v;
        T g = y - static_cast<T>(0.39465) * u - static_cast<T>(0.58060) * v;
        T b = y + static_cast<T>(2.03211) * u;
        return ColorRGB<T>(r, g, b);
    }
};

} // namespace qmath
