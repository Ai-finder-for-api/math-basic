/**
 * @file Vec4.hpp
 * @brief A comprehensive C++ implementation of a 4D Vector.
 * 
 * Used extensively in 3D graphics for homogeneous coordinates and color
 * representations (RGBA). Includes SIMD optimizations where applicable.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Constants.hpp"
#include "../core/Simd.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Vec4 {
    T x, y, z, w;

    // ---------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------
    Vec4() : x(0), y(0), z(0), w(0) {}
    Vec4(T xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) {}
    Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    Vec4(const Vec3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

    template<typename U>
    explicit Vec4(const Vec4<U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {}

    // ---------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------
    Vec4 operator+() const { return *this; }
    Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Vec4 + Vec4)
    // ---------------------------------------------------------
    Vec4 operator+(const Vec4& o) const { return Vec4(x + o.x, y + o.y, z + o.z, w + o.w); }
    Vec4 operator-(const Vec4& o) const { return Vec4(x - o.x, y - o.y, z - o.z, w - o.w); }
    Vec4 operator*(const Vec4& o) const { return Vec4(x * o.x, y * o.y, z * o.z, w * o.w); }
    Vec4 operator/(const Vec4& o) const { return Vec4(x / o.x, y / o.y, z / o.z, w / o.w); }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Vec4 + Scalar)
    // ---------------------------------------------------------
    Vec4 operator+(T s) const { return Vec4(x + s, y + s, z + s, w + s); }
    Vec4 operator-(T s) const { return Vec4(x - s, y - s, z - s, w - s); }
    Vec4 operator*(T s) const { return Vec4(x * s, y * s, z * s, w * s); }
    Vec4 operator/(T s) const { return Vec4(x / s, y / s, z / s, w / s); }

    // ---------------------------------------------------------
    // Assignment Operators
    // ---------------------------------------------------------
    Vec4& operator+=(const Vec4& o) { x += o.x; y += o.y; z += o.z; w += o.w; return *this; }
    Vec4& operator-=(const Vec4& o) { x -= o.x; y -= o.y; z -= o.z; w -= o.w; return *this; }
    Vec4& operator*=(const Vec4& o) { x *= o.x; y *= o.y; z *= o.z; w *= o.w; return *this; }
    Vec4& operator/=(const Vec4& o) { x /= o.x; y /= o.y; z /= o.z; w /= o.w; return *this; }
    
    Vec4& operator+=(T s) { x += s; y += s; z += s; w += s; return *this; }
    Vec4& operator-=(T s) { x -= s; y -= s; z -= s; w -= s; return *this; }
    Vec4& operator*=(T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    Vec4& operator/=(T s) { x /= s; y /= s; z /= s; w /= s; return *this; }

    // ---------------------------------------------------------
    // Comparison Operators
    // ---------------------------------------------------------
    bool operator==(const Vec4& o) const { return x == o.x && y == o.y && z == o.z && w == o.w; }
    bool operator!=(const Vec4& o) const { return !(*this == o); }
    bool operator<(const Vec4& o) const { return length_sq() < o.length_sq(); }
    bool operator>(const Vec4& o) const { return length_sq() > o.length_sq(); }

    // ---------------------------------------------------------
    // Geometric Properties
    // ---------------------------------------------------------
    T length_sq() const { return x * x + y * y + z * z + w * w; }
    T length() const { return std::sqrt(length_sq()); }
    
    T dot(const Vec4& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }

    T distance_sq(const Vec4& o) const { 
        T dx = x - o.x; 
        T dy = y - o.y; 
        T dz = z - o.z;
        T dw = w - o.w;
        return dx * dx + dy * dy + dz * dz + dw * dw; 
    }
    
    T distance(const Vec4& o) const { return std::sqrt(distance_sq(o)); }

    // ---------------------------------------------------------
    // Vector Operations
    // ---------------------------------------------------------
    Vec4 normalized() const {
        T len = length();
        if (len == 0) return Vec4(0, 0, 0, 0);
        return Vec4(x / len, y / len, z / len, w / len);
    }

    void normalize() {
        *this = normalized();
    }

    T angle_between(const Vec4& o) const {
        T dot_prod = dot(o);
        T len_prod = length() * o.length();
        if (len_prod == 0) return 0;
        return std::acos(clamp_val(dot_prod / len_prod, static_cast<T>(-1), static_cast<T>(1)));
    }

    // ---------------------------------------------------------
    // Interpolation
    // ---------------------------------------------------------
    Vec4 lerp(const Vec4& o, T t) const {
        return Vec4(x + (o.x - x) * t, y + (o.y - y) * t, z + (o.z - z) * t, w + (o.w - w) * t);
    }

    Vec4 abs() const {
        return Vec4(std::fabs(x), std::fabs(y), std::fabs(z), std::fabs(w));
    }
};

// Type Aliases
using Vec4f = Vec4<float>;
using Vec4d = Vec4<double>;
using Vec4i = Vec4<int>;

} // namespace qmath
