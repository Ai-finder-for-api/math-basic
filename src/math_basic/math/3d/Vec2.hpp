/**
 * @file Vec2.hpp
 * @brief A comprehensive C++ implementation of a 2D Vector.
 * 
 * Supports all standard arithmetic operations, geometric calculations
 * (dot, cross, length, distance), and interpolations.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Constants.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Vec2 {
    T x, y;

    // ---------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------
    Vec2() : x(0), y(0) {}
    Vec2(T xy) : x(xy), y(xy) {}
    Vec2(T x, T y) : x(x), y(y) {}
    
    template<typename U>
    explicit Vec2(const Vec2<U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

    // ---------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------
    Vec2 operator+() const { return *this; }
    Vec2 operator-() const { return Vec2(-x, -y); }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Vec2 + Vec2)
    // ---------------------------------------------------------
    Vec2 operator+(const Vec2& o) const { return Vec2(x + o.x, y + o.y); }
    Vec2 operator-(const Vec2& o) const { return Vec2(x - o.x, y - o.y); }
    Vec2 operator*(const Vec2& o) const { return Vec2(x * o.x, y * o.y); }
    Vec2 operator/(const Vec2& o) const { return Vec2(x / o.x, y / o.y); }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Vec2 + Scalar)
    // ---------------------------------------------------------
    Vec2 operator+(T s) const { return Vec2(x + s, y + s); }
    Vec2 operator-(T s) const { return Vec2(x - s, y - s); }
    Vec2 operator*(T s) const { return Vec2(x * s, y * s); }
    Vec2 operator/(T s) const { return Vec2(x / s, y / s); }

    // ---------------------------------------------------------
    // Assignment Operators
    // ---------------------------------------------------------
    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }
    Vec2& operator-=(const Vec2& o) { x -= o.x; y -= o.y; return *this; }
    Vec2& operator*=(const Vec2& o) { x *= o.x; y *= o.y; return *this; }
    Vec2& operator/=(const Vec2& o) { x /= o.x; y /= o.y; return *this; }
    
    Vec2& operator+=(T s) { x += s; y += s; return *this; }
    Vec2& operator-=(T s) { x -= s; y -= s; return *this; }
    Vec2& operator*=(T s) { x *= s; y *= s; return *this; }
    Vec2& operator/=(T s) { x /= s; y /= s; return *this; }

    // ---------------------------------------------------------
    // Comparison Operators
    // ---------------------------------------------------------
    bool operator==(const Vec2& o) const { return x == o.x && y == o.y; }
    bool operator!=(const Vec2& o) const { return !(*this == o); }
    bool operator<(const Vec2& o) const { return length_sq() < o.length_sq(); }
    bool operator>(const Vec2& o) const { return length_sq() > o.length_sq(); }

    // ---------------------------------------------------------
    // Geometric Properties
    // ---------------------------------------------------------
    T length_sq() const { return x * x + y * y; }
    T length() const { return std::sqrt(length_sq()); }
    
    T dot(const Vec2& o) const { return x * o.x + y * o.y; }
    
    // 2D Cross product (returns scalar representing Z component)
    T cross(const Vec2& o) const { return x * o.y - y * o.x; }

    T distance_sq(const Vec2& o) const { 
        T dx = x - o.x; 
        T dy = y - o.y; 
        return dx * dx + dy * dy; 
    }
    
    T distance(const Vec2& o) const { return std::sqrt(distance_sq(o)); }

    // ---------------------------------------------------------
    // Vector Operations
    // ---------------------------------------------------------
    Vec2 normalized() const {
        T len = length();
        if (len == 0) return Vec2(0, 0);
        return Vec2(x / len, y / len);
    }

    void normalize() {
        *this = normalized();
    }

    Vec2 perpendicular() const { return Vec2(-y, x); } // Rotate 90 deg counter-clockwise
    
    Vec2 rotate(T angle_rad) const {
        T c = std::cos(angle_rad);
        T s = std::sin(angle_rad);
        return Vec2(x * c - y * s, x * s + y * c);
    }

    T angle() const { return std::atan2(y, x); }
    
    T angle_between(const Vec2& o) const {
        T dot_prod = dot(o);
        T len_prod = length() * o.length();
        if (len_prod == 0) return 0;
        return std::acos(clamp_val(dot_prod / len_prod, static_cast<T>(-1), static_cast<T>(1)));
    }

    // ---------------------------------------------------------
    // Interpolation
    // ---------------------------------------------------------
    Vec2 lerp(const Vec2& o, T t) const {
        return Vec2(x + (o.x - x) * t, y + (o.y - y) * t);
    }

    Vec2 reflect(const Vec2& normal) const {
        T d = dot(normal);
        return *this - normal * (static_cast<T>(2) * d);
    }

    Vec2 abs() const {
        return Vec2(std::fabs(x), std::fabs(y));
    }
};

// Type Aliases
using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned int>;

} // namespace qmath
