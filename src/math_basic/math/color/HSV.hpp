/**
 * @file HSV.hpp
 * @brief HSV (Hue, Saturation, Value) Color Model.
 * 
 * Provides robust conversions between RGB and HSV color spaces.
 */
#pragma once
#include "RGB.hpp"
#include "../core/Utils.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct ColorHSV {
    T h; // Hue in degrees [0, 360) or [0, 1) depending on convention. We use [0, 1)
    T s; // Saturation [0, 1]
    T v; // Value [0, 1]

    ColorHSV() : h(0), s(0), v(0) {}
    ColorHSV(T h, T s, T v) : h(h), s(s), v(v) {}

    // Convert HSV to RGB
    ColorRGB<T> to_rgb() const {
        ColorRGB<T> rgb;
        T h60 = h * static_cast<T>(6.0);
        T h60f = std::floor(h60);
        int i = static_cast<int>(h60f) % 6;
        T f = h60 - h60f;
        T p = v * (static_cast<T>(1) - s);
        T q = v * (static_cast<T>(1) - f * s);
        T t = v * (static_cast<T>(1) - (static_cast<T>(1) - f) * s);

        switch (i) {
            case 0: rgb.r = v; rgb.g = t; rgb.b = p; break;
            case 1: rgb.r = q; rgb.g = v; rgb.b = p; break;
            case 2: rgb.r = p; rgb.g = v; rgb.b = t; break;
            case 3: rgb.r = p; rgb.g = q; rgb.b = v; break;
            case 4: rgb.r = t; rgb.g = p; rgb.b = v; break;
            case 5: rgb.r = v; rgb.g = p; rgb.b = q; break;
        }
        return rgb;
    }

    // Convert RGB to HSV
    static ColorHSV from_rgb(const ColorRGB<T>& rgb) {
        ColorHSV hsv;
        T max_c = rgb.max_component();
        T min_c = rgb.min_component();
        T delta = max_c - min_c;

        hsv.v = max_c;
        if (max_c > static_cast<T>(0)) {
            hsv.s = delta / max_c;
        } else {
            hsv.s = static_cast<T>(0);
            hsv.h = static_cast<T>(0); // Undefined
            return hsv;
        }

        if (delta <= static_cast<T>(1e-6)) {
            hsv.h = static_cast<T>(0); // Achromatic (gray)
            return hsv;
        }

        if (max_c == rgb.r) {
            hsv.h = (rgb.g - rgb.b) / delta;
            if (hsv.h < static_cast<T>(0)) hsv.h += static_cast<T>(6.0);
        } else if (max_c == rgb.g) {
            hsv.h = static_cast<T>(2.0) + (rgb.b - rgb.r) / delta;
        } else {
            hsv.h = static_cast<T>(4.0) + (rgb.r - rgb.g) / delta;
        }

        hsv.h /= static_cast<T>(6.0); // Normalize to [0, 1)
        return hsv;
    }
};

using ColorHSVf = ColorHSV<float>;

} // namespace qmath
