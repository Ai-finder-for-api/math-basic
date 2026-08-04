/**
 * @file AABB.hpp
 * @brief Represents an Axis-Aligned Bounding Box (AABB).
 * 
 * The workhorse of physics engines and renderers. Extremely fast collision
 * and ray intersection tests.
 */
#pragma once
#include "Vec3.hpp"
#include "Ray.hpp"
#include "../core/Utils.hpp"
#include <cmath>
#include <algorithm>

namespace qmath {

template<typename T>
struct AABB {
    Vec3<T> min;
    Vec3<T> max;

    AABB() : min(-1,-1,-1), max(1,1,1) {}
    AABB(const Vec3<T>& min, const Vec3<T>& max) : min(min), max(max) {}

    Vec3<T> center() const {
        return (min + max) * static_cast<T>(0.5);
    }

    Vec3<T> extents() const {
        return (max - min) * static_cast<T>(0.5);
    }

    // Expand the box to include a point
    void encapsulate(const Vec3<T>& p) {
        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        min.z = std::min(min.z, p.z);
        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
        max.z = std::max(max.z, p.z);
    }

    // Expand the box to include another box
    void encapsulate(const AABB& o) {
        encapsulate(o.min);
        encapsulate(o.max);
    }

    // Check if a point is inside the box
    bool contains(const Vec3<T>& p) const {
        return p.x >= min.x && p.x <= max.x &&
               p.y >= min.y && p.y <= max.y &&
               p.z >= min.z && p.z <= max.z;
    }

    // AABB-AABB Intersection
    bool intersects(const AABB& o) const {
        return !(o.min.x > max.x || o.max.x < min.x ||
                 o.min.y > max.y || o.max.y < min.y ||
                 o.min.z > max.z || o.max.z < min.z);
    }

    // Ray-AABB Intersection (Slab Method)
    // Returns the distance t to the first hit, or -1 if no hit
    T intersect_ray(const Ray<T>& ray) const {
        T tmin = static_cast<T>(0);
        T tmax = std::numeric_limits<T>::infinity();

        for (int i = 0; i < 3; ++i) {
            T inv_d = static_cast<T>(1) / ray.direction[i];
            T t1 = (min[i] - ray.origin[i]) * inv_d;
            T t2 = (max[i] - ray.origin[i]) * inv_d;

            if (t1 > t2) std::swap(t1, t2);

            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);

            if (tmin > tmax) return -1;
        }

        return tmin;
    }
};

} // namespace qmath
