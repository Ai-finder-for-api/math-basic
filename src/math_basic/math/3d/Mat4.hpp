/**
 * @file Mat4.hpp
 * @brief A comprehensive C++ implementation of a 4x4 Matrix.
 * 
 * The core of 3D graphics and physics engines. Supports homogeneous
 * coordinates, full affine transformations, camera projections,
 * determinants, and inverses.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Constants.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Mat4 {
    // Row-major storage: m[row][col]
    T m[4][4];

    // ---------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------
    Mat4() { 
        for(int i=0; i<4; ++i) 
            for(int j=0; j<4; ++j) 
                m[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0); 
    }

    Mat4(T m00, T m01, T m02, T m03,
         T m10, T m11, T m12, T m13,
         T m20, T m21, T m22, T m23,
         T m30, T m31, T m32, T m33) {
        m[0][0]=m00; m[0][1]=m01; m[0][2]=m02; m[0][3]=m03;
        m[1][0]=m10; m[1][1]=m11; m[1][2]=m12; m[1][3]=m13;
        m[2][0]=m20; m[2][1]=m21; m[2][2]=m22; m[2][3]=m23;
        m[3][0]=m30; m[3][1]=m31; m[3][2]=m32; m[3][3]=m33;
    }

    // ---------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------
    Mat4 operator+() const { return *this; }
    Mat4 operator-() const {
        Mat4 r;
        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                r.m[i][j] = -m[i][j];
        return r;
    }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Mat4 + Mat4)
    // ---------------------------------------------------------
    Mat4 operator+(const Mat4& o) const {
        Mat4 r;
        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                r.m[i][j] = m[i][j] + o.m[i][j];
        return r;
    }

    Mat4 operator-(const Mat4& o) const {
        Mat4 r;
        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                r.m[i][j] = m[i][j] - o.m[i][j];
        return r;
    }

    Mat4 operator*(const Mat4& o) const {
        Mat4 r;
        for(int i=0; i<4; ++i) {
            for(int j=0; j<4; ++j) {
                r.m[i][j] = m[i][0]*o.m[0][j] + m[i][1]*o.m[1][j] + m[i][2]*o.m[2][j] + m[i][3]*o.m[3][j];
            }
        }
        return r;
    }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Mat4 + Scalar)
    // ---------------------------------------------------------
    Mat4 operator+(T s) const {
        Mat4 r;
        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                r.m[i][j] = m[i][j] + s;
        return r;
    }

    Mat4 operator*(T s) const {
        Mat4 r;
        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j)
                r.m[i][j] = m[i][j] * s;
        return r;
    }

    // ---------------------------------------------------------
    // Matrix-Vector Multiplication
    // ---------------------------------------------------------
    Vec4<T> operator*(const Vec4<T>& v) const {
        return Vec4<T>(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w,
            m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w
        );
    }

    Vec3<T> transform_point(const Vec3<T>& v) const {
        Vec4<T> r = *this * Vec4<T>(v, static_cast<T>(1));
        if (r.w != 0 && r.w != 1) {
            return Vec3<T>(r.x / r.w, r.y / r.w, r.z / r.w);
        }
        return Vec3<T>(r.x, r.y, r.z);
    }

    Vec3<T> transform_direction(const Vec3<T>& v) const {
        Vec4<T> r = *this * Vec4<T>(v, static_cast<T>(0));
        return Vec3<T>(r.x, r.y, r.z);
    }

    // ---------------------------------------------------------
    // Assignment Operators
    // ---------------------------------------------------------
    Mat4& operator+=(const Mat4& o) { *this = *this + o; return *this; }
    Mat4& operator-=(const Mat4& o) { *this = *this - o; return *this; }
    Mat4& operator*=(const Mat4& o) { *this = *this * o; return *this; }
    Mat4& operator+=(T s) { *this = *this + s; return *this; }
    Mat4& operator*=(T s) { *this = *this * s; return *this; }

    // ---------------------------------------------------------
    // Matrix Properties
    // ---------------------------------------------------------
    Mat4 transposed() const {
        return Mat4(
            m[0][0], m[1][0], m[2][0], m[3][0],
            m[0][1], m[1][1], m[2][1], m[3][1],
            m[0][2], m[1][2], m[2][2], m[3][2],
            m[0][3], m[1][3], m[2][3], m[3][3]
        );
    }

    // Determinant calculation for 4x4 matrix
    T determinant() const {
        // Using Laplace expansion along the first row
        T det = 0;
        det += m[0][0] * Mat3<T>(
            m[1][1], m[1][2], m[1][3],
            m[2][1], m[2][2], m[2][3],
            m[3][1], m[3][2], m[3][3]
        ).determinant();
        
        det -= m[0][1] * Mat3<T>(
            m[1][0], m[1][2], m[1][3],
            m[2][0], m[2][2], m[2][3],
            m[3][0], m[3][2], m[3][3]
        ).determinant();
        
        det += m[0][2] * Mat3<T>(
            m[1][0], m[1][1], m[1][3],
            m[2][0], m[2][1], m[2][3],
            m[3][0], m[3][1], m[3][3]
        ).determinant();
        
        det -= m[0][3] * Mat3<T>(
            m[1][0], m[1][1], m[1][2],
            m[2][0], m[2][1], m[2][2],
            m[3][0], m[3][1], m[3][2]
        ).determinant();
        
        return det;
    }

    // ---------------------------------------------------------
    // Transformation Builders
    // ---------------------------------------------------------
    static Mat4 identity() { return Mat4(); }

    static Mat4 translation(T x, T y, T z) {
        Mat4 r = identity();
        r.m[0][3] = x;
        r.m[1][3] = y;
        r.m[2][3] = z;
        return r;
    }

    static Mat4 translation(const Vec3<T>& v) {
        return translation(v.x, v.y, v.z);
    }

    static Mat4 scaling(T x, T y, T z) {
        Mat4 r = identity();
        r.m[0][0] = x;
        r.m[1][1] = y;
        r.m[2][2] = z;
        return r;
    }

    static Mat4 scaling(const Vec3<T>& v) {
        return scaling(v.x, v.y, v.z);
    }

    static Mat4 rotation_x(T angle_rad) {
        Mat4 r = identity();
        T c = std::cos(angle_rad);
        T s = std::sin(angle_rad);
        r.m[1][1] = c; r.m[1][2] = -s;
        r.m[2][1] = s; r.m[2][2] = c;
        return r;
    }

    static Mat4 rotation_y(T angle_rad) {
        Mat4 r = identity();
        T c = std::cos(angle_rad);
        T s = std::sin(angle_rad);
        r.m[0][0] = c; r.m[0][2] = s;
        r.m[2][0] = -s; r.m[2][2] = c;
        return r;
    }

    static Mat4 rotation_z(T angle_rad) {
        Mat4 r = identity();
        T c = std::cos(angle_rad);
        T s = std::sin(angle_rad);
        r.m[0][0] = c; r.m[0][1] = -s;
        r.m[1][0] = s; r.m[1][1] = c;
        return r;
    }

    // Right-handed look-at matrix (OpenGL style)
    static Mat4 look_at(const Vec3<T>& eye, const Vec3<T>& center, const Vec3<T>& up) {
        Vec3<T> f = (center - eye).normalized(); // Forward
        Vec3<T> s = f.cross(up).normalized();    // Right
        Vec3<T> u = s.cross(f);                  // True Up

        return Mat4(
             s.x,  s.y,  s.z, -s.dot(eye),
             u.x,  u.y,  u.z, -u.dot(eye),
            -f.x, -f.y, -f.z,  f.dot(eye),
             0,    0,    0,    1
        );
    }

    // Right-handed perspective projection matrix (OpenGL style)
    static Mat4 perspective(T fovy_rad, T aspect, T z_near, T z_far) {
        T f = static_cast<T>(1) / std::tan(fovy_rad / static_cast<T>(2));
        T range_inv = static_cast<T>(1) / (z_near - z_far);

        return Mat4(
            f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (z_near + z_far) * range_inv, static_cast<T>(2) * z_near * z_far * range_inv,
            0, 0, -1, 0
        );
    }

    // Right-handed orthographic projection matrix (OpenGL style)
    static Mat4 ortho(T left, T right, T bottom, T top, T z_near, T z_far) {
        T rl = static_cast<T>(1) / (right - left);
        T tb = static_cast<T>(1) / (top - bottom);
        T fn = static_cast<T>(1) / (z_far - z_near);

        return Mat4(
            2 * rl, 0, 0, -(right + left) * rl,
            0, 2 * tb, 0, -(top + bottom) * tb,
            0, 0, -2 * fn, -(z_far + z_near) * fn,
            0, 0, 0, 1
        );
    }
};

// Type Aliases
using Mat4f = Mat4<float>;
using Mat4d = Mat4<double>;

} // namespace qmath
