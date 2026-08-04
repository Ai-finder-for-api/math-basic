/**
 * @file Sphere.hpp
 * @brief Represents a 3D Bounding Sphere.
 * 
 * Used for fast collision detection and culling. Includes ray intersection
 * and sphere-sphere intersection tests.
 */
#pragma once
#include "Vec3.hpp"
#include "Ray.hpp"
#include "../core/Utils.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Sphere {
    Vec3<T> center;
    T radius;

    Sphere() : center(0,0,0), radius(1) {}
    Sphere(const Vec3<T>& c, T r) : center(c), radius(r) {}

    // Check if a point is inside the sphere
    bool contains(const Vec3<T>& p) const {
        return center.distance_sq(p) <= radius * radius;
    }

    // Sphere-Sphere Intersection
    bool intersects(const Sphere& o) const {
        T dist_sq = center.distance_sq(o.center);
        T sum_radii = radius + o.radius;
        return dist_sq <= sum_radii * sum_radii;
    }

    // Ray-Sphere Intersection
    // Returns the closest distance t along the ray, or -1 if no intersection
    T intersect_ray(const Ray<T>& ray) const {
        Vec3<T> oc = ray.origin - center;
        T a = ray.direction.dot(ray.direction); // Should be 1 if normalized
        T b = static_cast<T>(2) * oc.dot(ray.direction);
        T c = oc.dot(oc) - radius * radius;
        
        T discriminant = b*b - static_cast<T>(4)*a*c;
        if (discriminant < static_cast<T>(0)) {
            return -1; // No intersection
        }
        
        T sqrt_disc = std::sqrt(discriminant);
        T t1 = (-b - sqrt_disc) / (static_cast<T>(2) * a);
        T t2 = (-b + sqrt_disc) / (static_cast<T>(2) * a);
        
        if (t1 > static_cast<T>(0)) return t1; // First hit is in front of ray
        if (t2 > static_cast<T>(0)) return t2; // Ray started inside sphere
        return -1; // Both hits are behind the ray
    }
};

} // namespace qmath
