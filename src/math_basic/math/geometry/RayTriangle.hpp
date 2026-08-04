/**
 * @file RayTriangle.hpp
 * @brief Möller–Trumbore Ray-Triangle Intersection Algorithm.
 * 
 * The standard method for determining if a 3D ray intersects a triangle.
 * Returns the distance `t` to the hit point, or -1 if missed.
 */
#pragma once
#include "../3d/Vec3.hpp"
#include "../3d/Ray.hpp"
#include "../core/Constants.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Triangle {
    Vec3<T> v0, v1, v2;

    Triangle() = default;
    Triangle(const Vec3<T>& a, const Vec3<T>& b, const Vec3<T>& c) : v0(a), v1(b), v2(c) {}

    // Returns distance t along ray, or -1 if no hit.
    T intersect_ray(const Ray<T>& ray, bool cull_back_faces = true) const {
        Vec3<T> edge1 = v1 - v0;
        Vec3<T> edge2 = v2 - v0;
        Vec3<T> h = ray.direction.cross(edge2);
        
        T a = edge1.dot(h);
        if (cull_back_faces && a < static_cast<T>(1e-8)) return -1; // Backface or parallel
        if (!cull_back_faces && std::fabs(a) < static_cast<T>(1e-8)) return -1; // Parallel

        T f = static_cast<T>(1) / a;
        Vec3<T> s = ray.origin - v0;
        T u = f * s.dot(h);
        if (u < 0.0 || u > 1.0) return -1;

        Vec3<T> q = s.cross(edge1);
        T v = f * ray.direction.dot(q);
        if (v < 0.0 || u + v > 1.0) return -1;

        T t = f * edge2.dot(q);
        if (t > static_cast<T>(1e-8)) return t; // Ray hit
        
        return -1; // Ray missed or line intersection but not ray
    }
};

} // namespace qmath
