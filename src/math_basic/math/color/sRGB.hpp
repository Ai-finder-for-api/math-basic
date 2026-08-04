/**
 * @file sRGB.hpp
 * @brief sRGB Gamma Encoding and Decoding.
 * 
 * Graphics rendering is done in Linear space, but images are stored in sRGB.
 * This file provides the math to convert between them accurately.
 */
#pragma once
#include "RGB.hpp"
#include "../core/Utils.hpp"
#include <cmath>

namespace qmath {

// Linear to sRGB encoding (Gamma 2.2 approximation with linear toe)
template<typename T>
inline T linear_to_srgb_component(T c) {
    if (c <= static_cast<T>(0.0031308)) {
        return c * static_cast<T>(12.92);
    }
    return static_cast<T>(1.055) * std::pow(c, static_cast<T>(1.0 / 2.4)) - static_cast<T>(0.055);
}

// sRGB to Linear decoding
template<typename T>
inline T srgb_to_linear_component(T c) {
    if (c <= static_cast<T>(0.04045)) {
        return c / static_cast<T>(12.92);
    }
    return std::pow((c + static_cast<T>(0.055)) / static_cast<T>(1.055), static_cast<T>(2.4));
}

template<typename T>
inline ColorRGB<T> linear_to_srgb(const ColorRGB<T>& linear_color) {
    return ColorRGB<T>(
        linear_to_srgb_component(linear_color.r),
        linear_to_srgb_component(linear_color.g),
        linear_to_srgb_component(linear_color.b)
    );
}

template<typename T>
inline ColorRGB<T> srgb_to_linear(const ColorRGB<T>& srgb_color) {
    return ColorRGB<T>(
        srgb_to_linear_component(srgb_color.r),
        srgb_to_linear_component(srgb_color.g),
        srgb_to_linear_component(srgb_color.b)
    );
}

} // namespace qmath
