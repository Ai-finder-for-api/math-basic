/**
 * @file Vec3.hpp
 * @brief A comprehensive C++ implementation of a 3D Vector.
 * 
 * The cornerstone of 3D mathematics, supporting all arithmetic operations,
 * dot/cross products, rotations, and geometric queries.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Constants.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Vec3 {
    T x, y, z;

    // ---------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------
    Vec3() : x(0), y(0), z(0) {}
    Vec3(T xyz) : x(xyz), y(xyz), z(xyz) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    template<typename U>
    explicit Vec3(const Vec3<U>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}

    // ---------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------
    Vec3 operator+() const { return *this; }
    Vec3 operator-() const { return Vec3(-x, -y, -z); }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Vec3 + Vec3)
    // ---------------------------------------------------------
    Vec3 operator+(const Vec3& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
    Vec3 operator*(const Vec3& o) const { return Vec3(x * o.x, y * o.y, z * o.z); }
    Vec3 operator/(const Vec3& o) const { return Vec3(x / o.x, y / o.y, z / o.z); }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Vec3 + Scalar)
    // ---------------------------------------------------------
    Vec3 operator+(T s) const { return Vec3(x + s, y + s, z + s); }
    Vec3 operator-(T s) const { return Vec3(x - s, y - s, z - s); }
    Vec3 operator*(T s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator/(T s) const { return Vec3(x / s, y / s, z / s); }

    // ---------------------------------------------------------
    // Assignment Operators
    // ---------------------------------------------------------
    Vec3& operator+=(const Vec3& o) { x += o.x; y += o.y; z += o.z; return *this; }
    Vec3& operator-=(const Vec3& o) { x -= o.x; y -= o.y; z -= o.z; return *this; }
    Vec3& operator*=(const Vec3& o) { x *= o.x; y *= o.y; z *= o.z; return *this; }
    Vec3& operator/=(const Vec3& o) { x /= o.x; y /= o.y; z /= o.z; return *this; }
    
    Vec3& operator+=(T s) { x += s; y += s; z += s; return *this; }
    Vec3& operator-=(T s) { x -= s; y -= s; z -= s; return *this; }
    Vec3& operator*=(T s) { x *= s; y *= s; z *= s; return *this; }
    Vec3& operator/=(T s) { x /= s; y /= s; z /= s; return *this; }

    // ---------------------------------------------------------
    // Comparison Operators
    // ---------------------------------------------------------
    bool operator==(const Vec3& o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const Vec3& o) const { return !(*this == o); }
    bool operator<(const Vec3& o) const { return length_sq() < o.length_sq(); }
    bool operator>(const Vec3& o) const { return length_sq() > o.length_sq(); }

    // ---------------------------------------------------------
    // Geometric Properties
    // ---------------------------------------------------------
    T length_sq() const { return x * x + y * y + z * z; }
    T length() const { return std::sqrt(length_sq()); }
    
    T dot(const Vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    
    Vec3 cross(const Vec3& o) const { 
        return Vec3(
            y * o.z - z * o.y,
            z * o.x - x * o.z,
            x * o.y - y * o.x
        ); 
    }

    T distance_sq(const Vec3& o) const { 
        T dx = x - o.x; 
        T dy = y - o.y; 
        T dz = z - o.z;
        return dx * dx + dy * dy + dz * dz; 
    }
    
    T distance(const Vec3& o) const { return std::sqrt(distance_sq(o)); }

    // ---------------------------------------------------------
    // Vector Operations
    // ---------------------------------------------------------
    Vec3 normalized() const {
        T len = length();
        if (len == 0) return Vec3(0, 0, 0);
        return Vec3(x / len, y / len, z / len);
    }

    void normalize() {
        *this = normalized();
    }

    T angle_between(const Vec3& o) const {
        T dot_prod = dot(o);
        T len_prod = length() * o.length();
        if (len_prod == 0) return 0;
        return std::acos(clamp_val(dot_prod / len_prod, static_cast<T>(-1), static_cast<T>(1)));
    }

    Vec3 reflect(const Vec3& normal) const {
        T d = dot(normal);
        return *this - normal * (static_cast<T>(2) * d);
    }

    // Refract incident vector. eta = ratio of indices of refraction
    Vec3 refract(const Vec3& normal, T eta) const {
        T cos_i = -dot(normal);
        T sin_t2 = eta * eta * (static_cast<T>(1) - cos_i * cos_i);
        if (sin_t2 > static_cast<T>(1)) return Vec3(0,0,0); // Total internal reflection
        T cos_t = std::sqrt(static_cast<T>(1) - sin_t2);
        return *this * eta + normal * (eta * cos_i - cos_t);
    }

    // Project this vector onto another
    Vec3 project(const Vec3& o) const {
        T denom = o.dot(o);
        if (denom == 0) return Vec3(0,0,0);
        return o * (dot(o) / denom);
    }

    // Orthogonal component of this vector relative to o
    Vec3 orthogonal_to(const Vec3& o) const {
        return *this - project(o);
    }

    // ---------------------------------------------------------
    // Interpolation
    // ---------------------------------------------------------
    Vec3 lerp(const Vec3& o, T t) const {
        return Vec3(x + (o.x - x) * t, y + (o.y - y) * t, z + (o.z - z) * t);
    }

    Vec3 abs() const {
        return Vec3(std::fabs(x), std::fabs(y), std::fabs(z));
    }
};

// Type Aliases
using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int>;

} // namespace qmath
