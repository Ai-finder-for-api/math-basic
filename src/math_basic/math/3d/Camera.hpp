/**
 * @file Camera.hpp
 * @brief A full 3D Camera class for graphics and ray tracing.
 * 
 * Manages view and projection matrices, and provides methods for
 * orbiting, panning, and zooming.
 */
#pragma once
#include "Vec3.hpp"
#include "Mat4.hpp"
#include "Quat.hpp"
#include "../core/Constants.hpp"
#include <cmath>

namespace qmath {

template<typename T>
class Camera {
public:
    Vec3<T> position;
    Quat<T> rotation;
    T fov; // in radians
    T aspect;
    T near_plane;
    T far_plane;

    Camera() : position(0, 0, 5), rotation(), fov(static_cast<T>(60.0 * PI_D / 180.0)), aspect(static_cast<T>(16.0 / 9.0)), near_plane(static_cast<T>(0.1)), far_plane(static_cast<T>(1000.0)) {}

    Mat4<T> get_view_matrix() const {
        // View matrix is the inverse of the camera's world transform
        Mat4<T> t = Mat4<T>::translation(-position);
        Mat4<T> r = rotation.conjugate().to_mat4();
        return r * t;
    }

    Mat4<T> get_projection_matrix() const {
        return Mat4<T>::perspective(fov, aspect, near_plane, far_plane);
    }

    Mat4<T> get_view_projection_matrix() const {
        return get_projection_matrix() * get_view_matrix();
    }

    // Get camera forward direction
    Vec3<T> forward() const {
        return rotation.rotate(Vec3<T>(0, 0, -1));
    }

    // Get camera right direction
    Vec3<T> right() const {
        return rotation.rotate(Vec3<T>(1, 0, 0));
    }

    // Get camera up direction
    Vec3<T> up() const {
        return rotation.rotate(Vec3<T>(0, 1, 0));
    }

    // Orbit around a target point
    void orbit(const Vec3<T>& target, T delta_yaw, T delta_pitch) {
        Vec3<T> offset = position - target;
        Quat<T> yaw_rot = Quat<T>::from_axis_angle(Vec3<T>(0, 1, 0), delta_yaw);
        Quat<T> pitch_rot = Quat<T>::from_axis_angle(Vec3<T>(1, 0, 0), delta_pitch);
        offset = yaw_rot.rotate(offset);
        offset = pitch_rot.rotate(offset);
        position = target + offset;
        
        // Look at target
        Vec3<T> dir = (target - position).normalized();
        // Calculate quaternion from direction (assuming up is Y)
        // This is a simplified look-at logic for orbiting
        Vec3<T> right = dir.cross(Vec3<T>(0, 1, 0)).normalized();
        Vec3<T> up = right.cross(dir).normalized();
        Mat3<T> rot_mat(
            right.x, up.x, -dir.x,
            right.y, up.y, -dir.y,
            right.z, up.z, -dir.z
        );
        rotation = Quat<T>::from_mat3(rot_mat).normalized();
    }
};

} // namespace qmath
