/**
 * @file Quat.hpp
 * @brief A comprehensive C++ implementation of a Quaternion.
 * 
 * Quaternions provide a mathematically robust way to represent 3D rotations
 * without suffering from gimbal lock. Supports SLERP, matrix conversion,
 * and Euler angle conversion.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Constants.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include "Mat3.hpp"
#include "Mat4.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Quat {
    T w, x, y, z;

    // ---------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------
    Quat() : w(1), x(0), y(0), z(0) {} // Identity quaternion
    Quat(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) {}
    
    // Construct from axis-angle
    static Quat from_axis_angle(const Vec3<T>& axis, T angle_rad) {
        Vec3<T> norm_axis = axis.normalized();
        T half_angle = angle_rad * static_cast<T>(0.5);
        T sin_half = std::sin(half_angle);
        T cos_half = std::cos(half_angle);
        return Quat(
            cos_half,
            norm_axis.x * sin_half,
            norm_axis.y * sin_half,
            norm_axis.z * sin_half
        );
    }

    // Construct from Euler angles (Pitch, Yaw, Roll) in radians
    static Quat from_euler(T pitch, T yaw, T roll) {
        T cy = std::cos(yaw * static_cast<T>(0.5));
        T sy = std::sin(yaw * static_cast<T>(0.5));
        T cp = std::cos(pitch * static_cast<T>(0.5));
        T sp = std::sin(pitch * static_cast<T>(0.5));
        T cr = std::cos(roll * static_cast<T>(0.5));
        T sr = std::sin(roll * static_cast<T>(0.5));

        return Quat(
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy
        );
    }

    // ---------------------------------------------------------
    // Operators
    // ---------------------------------------------------------
    Quat operator*(const Quat& o) const {
        return Quat(
            w*o.w - x*o.x - y*o.y - z*o.z,
            w*o.x + x*o.w + y*o.z - z*o.y,
            w*o.y - x*o.z + y*o.w + z*o.x,
            w*o.z + x*o.y - y*o.x + z*o.w
        );
    }

    Quat operator*(T s) const { return Quat(w*s, x*s, y*s, z*s); }
    Quat operator+(const Quat& o) const { return Quat(w+o.w, x+o.x, y+o.y, z+o.z); }
    Quat operator-() const { return Quat(-w, -x, -y, -z); }

    // ---------------------------------------------------------
    // Properties
    // ---------------------------------------------------------
    T length_sq() const { return w*w + x*x + y*y + z*z; }
    T length() const { return std::sqrt(length_sq()); }
    
    Quat normalized() const {
        T len = length();
        if (len == 0) return Quat(1, 0, 0, 0);
        return Quat(w/len, x/len, y/len, z/len);
    }

    Quat conjugate() const { return Quat(w, -x, -y, -z); }
    
    // Inverse for unit quaternions is just the conjugate
    Quat inverse() const {
        T len_sq = length_sq();
        if (len_sq == 0) return Quat(1, 0, 0, 0);
        return Quat(w/len_sq, -x/len_sq, -y/len_sq, -z/len_sq);
    }

    T dot(const Quat& o) const { return w*o.w + x*o.x + y*o.y + z*o.z; }

    // ---------------------------------------------------------
    // Rotations
    // ---------------------------------------------------------
    // Rotate a 3D vector by this quaternion
    Vec3<T> rotate(const Vec3<T>& v) const {
        // Optimized rotation: v' = v + 2.0 * cross(q.xyz, v + cross(q.xyz, v))
        Vec3<T> q_vec(x, y, z);
        Vec3<T> t = q_vec.cross(v) * static_cast<T>(2);
        Vec3<T> cross_q_t = q_vec.cross(t);
        return v + t * w + cross_q_t * static_cast<T>(2);
    }

    // Convert to 3x3 Rotation Matrix
    Mat3<T> to_mat3() const {
        T xx = x * x;
        T yy = y * y;
        T zz = z * z;
        T xy = x * y;
        T xz = x * z;
        T yz = y * z;
        T wx = w * x;
        T wy = w * y;
        T wz = w * z;

        return Mat3<T>(
            static_cast<T>(1) - static_cast<T>(2) * (yy + zz), static_cast<T>(2) * (xy - wz), static_cast<T>(2) * (xz + wy),
            static_cast<T>(2) * (xy + wz), static_cast<T>(1) - static_cast<T>(2) * (xx + zz), static_cast<T>(2) * (yz - wx),
            static_cast<T>(2) * (xz - wy), static_cast<T>(2) * (yz + wx), static_cast<T>(1) - static_cast<T>(2) * (xx + yy)
        );
    }

    // Convert to 4x4 Rotation Matrix
    Mat4<T> to_mat4() const {
        Mat3<T> rot = to_mat3();
        return Mat4<T>(
            rot.m[0][0], rot.m[0][1], rot.m[0][2], 0,
            rot.m[1][0], rot.m[1][1], rot.m[1][2], 0,
            rot.m[2][0], rot.m[2][1], rot.m[2][2], 0,
            0, 0, 0, 1
        );
    }

    // ---------------------------------------------------------
    // Interpolation
    // ---------------------------------------------------------
    // Spherical Linear Interpolation
    static Quat slerp(const Quat& q1, const Quat& q2, T t) {
        Quat q2_ = q2;
        T cos_theta = q1.dot(q2_);

        // If the dot product is negative, slerp won't take the shorter path.
        if (cos_theta < static_cast<T>(0)) {
            q2_ = -q2_;
            cos_theta = -cos_theta;
        }

        // If the quaternions are close enough, use linear interpolation
        if (cos_theta > static_cast<T>(0.9995)) {
            return Quat(
                q1.w + t * (q2_.w - q1.w),
                q1.x + t * (q2_.x - q1.x),
                q1.y + t * (q2_.y - q1.y),
                q1.z + t * (q2_.z - q1.z)
            ).normalized();
        }

        T theta = std::acos(cos_theta);
        T sin_theta = std::sin(theta);
        T t1 = std::sin((static_cast<T>(1) - t) * theta) / sin_theta;
        T t2 = std::sin(t * theta) / sin_theta;

        return Quat(
            q1.w * t1 + q2_.w * t2,
            q1.x * t1 + q2_.x * t2,
            q1.y * t1 + q2_.y * t2,
            q1.z * t1 + q2_.z * t2
        );
    }
};

// Type Aliases
using Quatf = Quat<float>;
using Quatd = Quat<double>;

} // namespace qmath
