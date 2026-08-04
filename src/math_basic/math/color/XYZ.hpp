/**
 * @file XYZ.hpp
 * @brief CIE 1931 XYZ Color Space.
 */
#pragma once
#include "RGB.hpp"

namespace qmath {

template<typename T>
struct ColorXYZ {
    T x, y, z;

    ColorXYZ(T x, T y, T z) : x(x), y(y), z(z) {}

    // sRGB to XYZ (D65 illuminant)
    static ColorXYZ from_rgb(const ColorRGB<T>& rgb) {
        T r = (rgb.r > static_cast<T>(0.04045)) ? std::pow((rgb.r + static_cast<T>(0.055)) / static_cast<T>(1.055), static_cast<T>(2.4)) : rgb.r / static_cast<T>(12.92);
        T g = (rgb.g > static_cast<T>(0.04045)) ? std::pow((rgb.g + static_cast<T>(0.055)) / static_cast<T>(1.055), static_cast<T>(2.4)) : rgb.g / static_cast<T>(12.92);
        T b = (rgb.b > static_cast<T>(0.04045)) ? std::pow((rgb.b + static_cast<T>(0.055)) / static_cast<T>(1.055), static_cast<T>(2.4)) : rgb.b / static_cast<T>(12.92);

        T x = r * static_cast<T>(0.4124) + g * static_cast<T>(0.3576) + b * static_cast<T>(0.1805);
        T y = r * static_cast<T>(0.2126) + g * static_cast<T>(0.7152) + b * static_cast<T>(0.0722);
        T z = r * static_cast<T>(0.0193) + g * static_cast<T>(0.1192) + b * static_cast<T>(0.9505);

        return ColorXYZ(x * static_cast<T>(100), y * static_cast<T>(100), z * static_cast<T>(100));
    }

    // XYZ to sRGB (D65 illuminant)
    ColorRGB<T> to_rgb() const {
        T x = this->x / static_cast<T>(100);
        T y = this->y / static_cast<T>(100);
        T z = this->z / static_cast<T>(100);

        T r = x * static_cast<T>(3.2406) + y * static_cast<T>(-1.5372) + z * static_cast<T>(-0.4986);
        T g = x * static_cast<T>(-0.9689) + y * static_cast<T>(1.8758) + z * static_cast<T>(0.0415);
        T b = x * static_cast<T>(0.0557) + y * static_cast<T>(-0.2040) + z * static_cast<T>(1.0570);

        auto gamma_correct = [](T c) {
            return (c > static_cast<T>(0.0031308)) ? static_cast<T>(1.055) * std::pow(c, static_cast<T>(1.0/2.4)) - static_cast<T>(0.055) : static_cast<T>(12.92) * c;
        };

        return ColorRGB<T>(gamma_correct(r), gamma_correct(g), gamma_correct(b));
    }
};

} // namespace qmath
