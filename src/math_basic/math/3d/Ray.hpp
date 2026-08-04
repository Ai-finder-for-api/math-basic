/**
 * @file Ray.hpp
 * @brief Represents a 3D Ray (origin and direction).
 * 
 * Used extensively in raycasting, picking, and ray-tracing intersections.
 */
#pragma once
#include "Vec3.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Ray {
    Vec3<T> origin;
    Vec3<T> direction; // Should be normalized

    Ray() : origin(0,0,0), direction(0,0,-1) {}
    Ray(const Vec3<T>& o, const Vec3<T>& d) : origin(o), direction(d.normalized()) {}

    // Get a point along the ray at distance t
    Vec3<T> at(T t) const {
        return origin + direction * t;
    }
};

} // namespace qmath
