/**
 * @file RGB.hpp
 * @brief A comprehensive C++ implementation of an RGB color model.
 * 
 * Uses floating-point precision (0.0 to 1.0) for high-dynamic-range (HDR)
 * compatibility. Supports all arithmetic operators and color-specific utilities.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Constants.hpp"
#include "../3d/Vec3.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct ColorRGB {
    T r, g, b;

    // ---------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------
    ColorRGB() : r(0), g(0), b(0) {}
    ColorRGB(T rgb) : r(rgb), g(rgb), b(rgb) {}
    ColorRGB(T r, T g, T b) : r(r), g(g), b(b) {}
    
    // Construct from a Vec3 (e.g., interpreting a normal as a color)
    explicit ColorRGB(const Vec3<T>& v) : r(v.x), g(v.y), b(v.z) {}

    // ---------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------
    ColorRGB operator+() const { return *this; }
    ColorRGB operator-() const { return ColorRGB(-r, -g, -b); }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Color + Color)
    // ---------------------------------------------------------
    ColorRGB operator+(const ColorRGB& o) const { return ColorRGB(r + o.r, g + o.g, b + o.b); }
    ColorRGB operator-(const ColorRGB& o) const { return ColorRGB(r - o.r, g - o.g, b - o.b); }
    ColorRGB operator*(const ColorRGB& o) const { return ColorRGB(r * o.r, g * o.g, b * o.b); }
    ColorRGB operator/(const ColorRGB& o) const { return ColorRGB(r / o.r, g / o.g, b / o.b); }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Color + Scalar)
    // ---------------------------------------------------------
    ColorRGB operator+(T s) const { return ColorRGB(r + s, g + s, b + s); }
    ColorRGB operator-(T s) const { return ColorRGB(r - s, g - s, b - s); }
    ColorRGB operator*(T s) const { return ColorRGB(r * s, g * s, b * s); }
    ColorRGB operator/(T s) const { return ColorRGB(r / s, g / s, b / s); }

    // ---------------------------------------------------------
    // Assignment Operators
    // ---------------------------------------------------------
    ColorRGB& operator+=(const ColorRGB& o) { r += o.r; g += o.g; b += o.b; return *this; }
    ColorRGB& operator-=(const ColorRGB& o) { r -= o.r; g -= o.g; b -= o.b; return *this; }
    ColorRGB& operator*=(const ColorRGB& o) { r *= o.r; g *= o.g; b *= o.b; return *this; }
    ColorRGB& operator/=(const ColorRGB& o) { r /= o.r; g /= o.g; b /= o.b; return *this; }
    
    ColorRGB& operator+=(T s) { r += s; g += s; b += s; return *this; }
    ColorRGB& operator-=(T s) { r -= s; g -= s; b -= s; return *this; }
    ColorRGB& operator*=(T s) { r *= s; g *= s; b *= s; return *this; }
    ColorRGB& operator/=(T s) { r /= s; g /= s; b /= s; return *this; }

    // ---------------------------------------------------------
    // Comparison Operators
    // ---------------------------------------------------------
    bool operator==(const ColorRGB& o) const { return r == o.r && g == o.g && b == o.b; }
    bool operator!=(const ColorRGB& o) const { return !(*this == o); }

    // ---------------------------------------------------------
    // Color Properties & Operations
    // ---------------------------------------------------------
    T luminance() const {
        // Standard BT.709 luma weights
        return static_cast<T>(0.2126) * r + static_cast<T>(0.7152) * g + static_cast<T>(0.0722) * b;
    }

    T max_component() const {
        return std::fmax(r, std::fmax(g, b));
    }

    T min_component() const {
        return std::fmin(r, std::fmin(g, b));
    }

    // Clamp components to [0, 1] range
    ColorRGB saturated() const {
        return ColorRGB(
            clamp_val(r, static_cast<T>(0), static_cast<T>(1)),
            clamp_val(g, static_cast<T>(0), static_cast<T>(1)),
            clamp_val(b, static_cast<T>(0), static_cast<T>(1))
        );
    }

    // Linear interpolation between colors
    ColorRGB lerp(const ColorRGB& o, T t) const {
        return ColorRGB(
            r + (o.r - r) * t,
            g + (o.g - g) * t,
            b + (o.b - b) * t
        );
    }

    // Convert to Vec3 for use in 3D math
    Vec3<T> to_vec3() const { return Vec3<T>(r, g, b); }
};

// Type Aliases
using ColorRGBf = ColorRGB<float>;
using ColorRGBd = ColorRGB<double>;

} // namespace qmath
