/**
 * @file Bezier.hpp
 * @brief Cubic and Quadratic Bezier Curve implementations.
 */
#pragma once
#include "Vec3.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct BezierCubic {
    Vec3<T> p0, p1, p2, p3;

    BezierCubic(const Vec3<T>& p0, const Vec3<T>& p1, const Vec3<T>& p2, const Vec3<T>& p3) : p0(p0), p1(p1), p2(p2), p3(p3) {}

    // Evaluate curve at t [0, 1]
    Vec3<T> evaluate(T t) const {
        T u = static_cast<T>(1) - t;
        T tt = t * t;
        T uu = u * u;
        T ttt = tt * t;
        T uuu = uu * u;
        
        return p0 * uuu + 
               p1 * (static_cast<T>(3) * uu * t) + 
               p2 * (static_cast<T>(3) * u * tt) + 
               p3 * ttt;
    }

    // First derivative (tangent)
    Vec3<T> derivative(T t) const {
        T u = static_cast<T>(1) - t;
        return (p1 - p0) * (static_cast<T>(3) * u * u) +
               (p2 - p1) * (static_cast<T>(6) * u * t) +
               (p3 - p2) * (static_cast<T>(3) * t * t);
    }
};

template<typename T>
struct BezierQuadratic {
    Vec3<T> p0, p1, p2;

    BezierQuadratic(const Vec3<T>& p0, const Vec3<T>& p1, const Vec3<T>& p2) : p0(p0), p1(p1), p2(p2) {}

    Vec3<T> evaluate(T t) const {
        T u = static_cast<T>(1) - t;
        return p0 * (u * u) + p1 * (static_cast<T>(2) * u * t) + p2 * (t * t);
    }

    Vec3<T> derivative(T t) const {
        T u = static_cast<T>(1) - t;
        return (p1 - p0) * (static_cast<T>(2) * u) + (p2 - p1) * (static_cast<T>(2) * t);
    }
};

} // namespace qmath
