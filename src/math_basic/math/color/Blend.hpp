/**
 * @file Blend.hpp
 * @brief Color Blending and Mixing Modes.
 * 
 * Implements standard Photoshop/GIMP style blending modes useful for
 * image processing and graphics rendering.
 */
#pragma once
#include "RGB.hpp"
#include "../core/Utils.hpp"
#include <algorithm>

namespace qmath {

template<typename T>
struct Blend {
    // Alpha blending: result = src * alpha + dst * (1 - alpha)
    static ColorRGB<T> normal(const ColorRGB<T>& src, const ColorRGB<T>& dst, T alpha) {
        return src * alpha + dst * (static_cast<T>(1) - alpha);
    }

    // Additive blending: result = src + dst
    static ColorRGB<T> additive(const ColorRGB<T>& src, const ColorRGB<T>& dst) {
        return src + dst;
    }

    // Multiply blending: result = src * dst
    static ColorRGB<T> multiply(const ColorRGB<T>& src, const ColorRGB<T>& dst) {
        return src * dst;
    }

    // Screen blending: result = 1 - (1 - src) * (1 - dst)
    static ColorRGB<T> screen(const ColorRGB<T>& src, const ColorRGB<T>& dst) {
        ColorRGB<T> inv_src(static_cast<T>(1) - src.r, static_cast<T>(1) - src.g, static_cast<T>(1) - src.b);
        ColorRGB<T> inv_dst(static_cast<T>(1) - dst.r, static_cast<T>(1) - dst.g, static_cast<T>(1) - dst.b);
        return ColorRGB<T>(static_cast<T>(1), static_cast<T>(1), static_cast<T>(1)) - (inv_src * inv_dst);
    }

    // Overlay blending: combination of multiply and screen
    static ColorRGB<T> overlay(const ColorRGB<T>& src, const ColorRGB<T>& dst) {
        ColorRGB<T> result;
        // Red channel
        if (dst.r < static_cast<T>(0.5)) {
            result.r = static_cast<T>(2) * src.r * dst.r;
        } else {
            result.r = static_cast<T>(1) - static_cast<T>(2) * (static_cast<T>(1) - src.r) * (static_cast<T>(1) - dst.r);
        }
        // Green channel
        if (dst.g < static_cast<T>(0.5)) {
            result.g = static_cast<T>(2) * src.g * dst.g;
        } else {
            result.g = static_cast<T>(1) - static_cast<T>(2) * (static_cast<T>(1) - src.g) * (static_cast<T>(1) - dst.g);
        }
        // Blue channel
        if (dst.b < static_cast<T>(0.5)) {
            result.b = static_cast<T>(2) * src.b * dst.b;
        } else {
            result.b = static_cast<T>(1) - static_cast<T>(2) * (static_cast<T>(1) - src.b) * (static_cast<T>(1) - dst.b);
        }
        return result;
    }
};

} // namespace qmath
