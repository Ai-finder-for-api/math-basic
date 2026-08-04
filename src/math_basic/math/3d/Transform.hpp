/**
 * @file Transform.hpp
 * @brief A comprehensive 3D Transform class.
 * 
 * Combines position, rotation (quaternion), and scale into a single
 * model matrix, similar to Unity or Unreal Engine transforms.
 */
#pragma once
#include "Vec3.hpp"
#include "Quat.hpp"
#include "Mat4.hpp"

namespace qmath {

template<typename T>
struct Transform {
    Vec3<T> position;
    Quat<T> rotation;
    Vec3<T> scale;

    Transform() : position(0,0,0), rotation(), scale(1,1,1) {}

    // Translate local position
    void translate(const Vec3<T>& delta) {
        position += delta;
    }

    // Rotate by a quaternion
    void rotate(const Quat<T>& q) {
        rotation = (q * rotation).normalized();
    }

    // Rotate by axis-angle
    void rotate(const Vec3<T>& axis, T angle_rad) {
        Quat<T> q = Quat<T>::from_axis_angle(axis, angle_rad);
        rotate(q);
    }

    // Scale uniformly
    void scale_by(T s) {
        scale *= s;
    }

    // Get the local model matrix (T * R * S)
    Mat4<T> to_matrix() const {
        Mat4<T> t = Mat4<T>::translation(position);
        Mat4<T> r = rotation.to_mat4();
        Mat4<T> s = Mat4<T>::scaling(scale);
        return t * r * s;
    }

    // Get the inverse model matrix (S^-1 * R^-1 * T^-1)
    Mat4<T> to_inverse_matrix() const {
        Mat4<T> t_inv = Mat4<T>::translation(-position);
        Mat4<T> r_inv = rotation.conjugate().to_mat4();
        Mat4<T> s_inv = Mat4<T>::scaling(Vec3<T>(static_cast<T>(1)/scale.x, static_cast<T>(1)/scale.y, static_cast<T>(1)/scale.z));
        return s_inv * r_inv * t_inv;
    }

    // Transform a point from local to world space
    Vec3<T> transform_point(const Vec3<T>& p) const {
        return to_matrix().transform_point(p);
    }

    // Transform a direction from local to world space (ignores scale and translation)
    Vec3<T> transform_direction(const Vec3<T>& d) const {
        return rotation.rotate(d);
    }
};

} // namespace qmath
