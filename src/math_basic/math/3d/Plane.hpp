/**
 * @file Plane.hpp
 * @brief Represents a 3D Plane.
 * 
 * Defined by a normal and a distance from the origin. Includes intersection
 * tests against rays and classification of points.
 */
#pragma once
#include "Vec3.hpp"
#include "Ray.hpp"
#include "../core/Utils.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Plane {
    Vec3<T> normal;
    T distance; // Distance from origin along the normal

    Plane() : normal(0, 1, 0), distance(0) {}
    Plane(const Vec3<T>& n, T d) : normal(n.normalized()), distance(d) {}
    
    // Construct from 3 points (counter-clockwise winding)
    Plane(const Vec3<T>& p1, const Vec3<T>& p2, const Vec3<T>& p3) {
        Vec3<T> v1 = p2 - p1;
        Vec3<T> v2 = p3 - p1;
        normal = v1.cross(v2).normalized();
        distance = normal.dot(p1);
    }

    // Distance from point to plane (positive = same side as normal, negative = opposite)
    T distance_to_point(const Vec3<T>& p) const {
        return normal.dot(p) - distance;
    }

    // Classify point relative to plane
    enum class Side {
        FRONT,
        BACK,
        ON_PLANE
    };

    Side classify_point(const Vec3<T>& p, T epsilon = static_cast<T>(1e-6)) const {
        T d = distance_to_point(p);
        if (d > epsilon) return Side::FRONT;
        if (d < -epsilon) return Side::BACK;
        return Side::ON_PLANE;
    }

    // Ray-Plane Intersection
    // Returns the distance t along the ray, or -1 if no intersection
    T intersect_ray(const Ray<T>& ray) const {
        T denom = normal.dot(ray.direction);
        if (std::fabs(denom) < static_cast<T>(1e-6)) {
            return -1; // Ray is parallel to plane
        }
        T t = (distance - normal.dot(ray.origin)) / denom;
        return t; // If t < 0, intersection is behind the ray origin
    }
};

} // namespace qmath
