/**
 * @file HSL.hpp
 * @brief HSL (Hue, Saturation, Lightness) Color Space.
 */
#pragma once
#include "RGB.hpp"
#include "../core/Utils.hpp"
#include <cmath>
#include <algorithm>

namespace qmath {

template<typename T>
struct ColorHSL {
    T h, s, l;

    ColorHSL(T h, T s, T l) : h(h), s(s), l(l) {}

    static ColorHSL from_rgb(const ColorRGB<T>& rgb) {
        T max_c = rgb.max_component();
        T min_c = rgb.min_component();
        T delta = max_c - min_c;

        ColorHSL hsl(0, 0, (max_c + min_c) / static_cast<T>(2));

        if (delta > static_cast<T>(1e-6)) {
            hsl.s = delta / (static_cast<T>(1) - std::fabs(static_cast<T>(2) * hsl.l - static_cast<T>(1)));
            
            if (max_c == rgb.r) {
                hsl.h = (rgb.g - rgb.b) / delta;
            } else if (max_c == rgb.g) {
                hsl.h = static_cast<T>(2) + (rgb.b - rgb.r) / delta;
            } else {
                hsl.h = static_cast<T>(4) + (rgb.r - rgb.g) / delta;
            }
            
            hsl.h /= static_cast<T>(6);
            if (hsl.h < static_cast<T>(0)) hsl.h += static_cast<T>(1);
        }
        return hsl;
    }

    ColorRGB<T> to_rgb() const {
        if (s < static_cast<T>(1e-6)) {
            return ColorRGB<T>(l, l, l);
        }
        
        T q = l < static_cast<T>(0.5) ? l * (static_cast<T>(1) + s) : l + s - l * s;
        T p = static_cast<T>(2) * l - q;
        
        auto hue_to_rgb = [](T p, T q, T t) {
            if (t < static_cast<T>(0)) t += static_cast<T>(1);
            if (t > static_cast<T>(1)) t -= static_cast<T>(1);
            if (t < static_cast<T>(1.0/6.0)) return p + (q - p) * static_cast<T>(6) * t;
            if (t < static_cast<T>(0.5)) return q;
            if (t < static_cast<T>(2.0/3.0)) return p + (q - p) * (static_cast<T>(2.0/3.0) - t) * static_cast<T>(6);
            return p;
        };
        
        T r = hue_to_rgb(p, q, h + static_cast<T>(1.0/3.0));
        T g = hue_to_rgb(p, q, h);
        T b = hue_to_rgb(p, q, h - static_cast<T>(1.0/3.0));
        
        return ColorRGB<T>(r, g, b);
    }
};

} // namespace qmath
