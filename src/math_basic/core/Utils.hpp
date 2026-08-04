/**
 * @file Utils.hpp
 * @brief General purpose mathematical utility functions.
 * 
 * Contains templated utility functions for comparisons, interpolation,
 * clamping, and power-of-two calculations. Designed for high performance
 * and cross-platform compatibility.
 */
#pragma once
#include <cmath>
#include <algorithm>
#include <type_traits>
#include <cstdint>

namespace qmath {

// -----------------------------------------------------------------
// Basic Min/Max/Clamp (Templates)
// -----------------------------------------------------------------
template<typename T>
inline T min_val(T a, T b) {
    return a < b ? a : b;
}

template<typename T>
inline T max_val(T a, T b) {
    return a > b ? a : b;
}

template<typename T>
inline T clamp_val(T val, T low, T high) {
    return val < low ? low : (val > high ? high : val);
}

// Specializations for float (using std::fmin/fmax for NaN safety)
template<>
inline float min_val(float a, float b) {
    return std::fmin(a, b);
}

template<>
inline float max_val(float a, float b) {
    return std::fmax(a, b);
}

template<>
inline float clamp_val(float val, float low, float high) {
    return std::fmax(low, std::fmin(val, high));
}

// -----------------------------------------------------------------
// Interpolation Functions
// -----------------------------------------------------------------
template<typename T>
inline T lerp(T a, T b, T t) {
    return a + (b - a) * t;
}

template<typename T>
inline T lerp_clamped(T a, T b, T t) {
    return lerp(a, b, clamp_val(t, static_cast<T>(0), static_cast<T>(1)));
}

// Smoothstep: t * t * (3 - 2 * t)
template<typename T>
inline T smoothstep(T t) {
    t = clamp_val(t, static_cast<T>(0), static_cast<T>(1));
    return t * t * (static_cast<T>(3) - static_cast<T>(2) * t);
}

// Smootherstep: t * t * t * (t * (t * 6 - 15) + 10)
template<typename T>
inline T smootherstep(T t) {
    t = clamp_val(t, static_cast<T>(0), static_cast<T>(1));
    return t * t * t * (t * (t * static_cast<T>(6) - static_cast<T>(15)) + static_cast<T>(10));
}

// -----------------------------------------------------------------
// Angle Conversion Functions
// -----------------------------------------------------------------
template<typename T>
inline T deg_to_rad(T deg) {
    return deg * (static_cast<T>(PI_D) / static_cast<T>(180.0));
}

template<typename T>
inline T rad_to_deg(T rad) {
    return rad * (static_cast<T>(180.0) / static_cast<T>(PI_D));
}

// Fast angle wrapping to [0, 2*PI)
template<typename T>
inline T wrap_angle_2pi(T angle) {
    constexpr T TWO_PI = static_cast<T>(2.0) * static_cast<T>(PI_D);
    while (angle < static_cast<T>(0)) angle += TWO_PI;
    while (angle >= TWO_PI) angle -= TWO_PI;
    return angle;
}

// Fast angle wrapping to [-PI, PI]
template<typename T>
inline T wrap_angle_pi(T angle) {
    constexpr T PI = static_cast<T>(PI_D);
    while (angle < -PI) angle += static_cast<T>(2.0) * PI;
    while (angle > PI) angle -= static_cast<T>(2.0) * PI;
    return angle;
}

// -----------------------------------------------------------------
// Comparison Functions (Epsilon-based for floating point)
// -----------------------------------------------------------------
template<typename T>
inline bool approx_equal(T a, T b, T epsilon = static_cast<T>(1e-6)) {
    return std::fabs(a - b) < epsilon;
}

template<typename T>
inline bool approx_zero(T a, T epsilon = static_cast<T>(1e-6)) {
    return std::fabs(a) < epsilon;
}

// -----------------------------------------------------------------
// Power of Two Utilities
// -----------------------------------------------------------------
inline bool is_power_of_two(uint32_t x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

inline uint32_t next_power_of_two(uint32_t x) {
    if (x == 0) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x++;
    return x;
}

inline uint32_t prev_power_of_two(uint32_t x) {
    if (x == 0) return 0;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x--;
    return x;
}

// -----------------------------------------------------------------
// Sign Functions
// -----------------------------------------------------------------
template<typename T>
inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

template<typename T>
inline T sign_val(T val) {
    return (val > T(0)) ? T(1) : ((val < T(0)) ? T(-1) : T(0));
}

// -----------------------------------------------------------------
// IsNaN and IsInf checks
// -----------------------------------------------------------------
template<typename T>
inline bool is_nan(T val) {
    return std::isnan(val);
}

template<typename T>
inline bool is_inf(T val) {
    return std::isinf(val);
}

template<typename T>
inline bool is_finite(T val) {
    return std::isfinite(val);
}

} // namespace qmath
