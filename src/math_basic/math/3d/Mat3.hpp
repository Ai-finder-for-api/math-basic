/**
 * @file Mat3.hpp
 * @brief A comprehensive C++ implementation of a 3x3 Matrix.
 * 
 * Designed for 2D transformations and 3D rotations. Includes full
 * arithmetic operations, determinant, inverse, and transformation builders.
 */
#pragma once
#include "../core/Utils.hpp"
#include "../core/Constants.hpp"
#include "Vec3.hpp"
#include "Vec2.hpp"
#include <cmath>

namespace qmath {

template<typename T>
struct Mat3 {
    // Row-major storage: m[row][col]
    T m[3][3];

    // ---------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------
    Mat3() { 
        for(int i=0; i<3; ++i) 
            for(int j=0; j<3; ++j) 
                m[i][j] = (i == j) ? static_cast<T>(1) : static_cast<T>(0); 
    }
    
    Mat3(T m00, T m01, T m02,
         T m10, T m11, T m12,
         T m20, T m21, T m22) {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
    }

    // ---------------------------------------------------------
    // Unary Operators
    // ---------------------------------------------------------
    Mat3 operator+() const { return *this; }
    Mat3 operator-() const {
        return Mat3(
            -m[0][0], -m[0][1], -m[0][2],
            -m[1][0], -m[1][1], -m[1][2],
            -m[2][0], -m[2][1], -m[2][2]
        );
    }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Mat3 + Mat3)
    // ---------------------------------------------------------
    Mat3 operator+(const Mat3& o) const {
        Mat3 r;
        for(int i=0; i<3; ++i)
            for(int j=0; j<3; ++j)
                r.m[i][j] = m[i][j] + o.m[i][j];
        return r;
    }

    Mat3 operator-(const Mat3& o) const {
        Mat3 r;
        for(int i=0; i<3; ++i)
            for(int j=0; j<3; ++j)
                r.m[i][j] = m[i][j] - o.m[i][j];
        return r;
    }

    Mat3 operator*(const Mat3& o) const {
        Mat3 r;
        for(int i=0; i<3; ++i) {
            for(int j=0; j<3; ++j) {
                r.m[i][j] = m[i][0]*o.m[0][j] + m[i][1]*o.m[1][j] + m[i][2]*o.m[2][j];
            }
        }
        return r;
    }

    // ---------------------------------------------------------
    // Binary Arithmetic Operators (Mat3 + Scalar)
    // ---------------------------------------------------------
    Mat3 operator+(T s) const {
        return Mat3(
            m[0][0]+s, m[0][1]+s, m[0][2]+s,
            m[1][0]+s, m[1][1]+s, m[1][2]+s,
            m[2][0]+s, m[2][1]+s, m[2][2]+s
        );
    }

    Mat3 operator*(T s) const {
        return Mat3(
            m[0][0]*s, m[0][1]*s, m[0][2]*s,
            m[1][0]*s, m[1][1]*s, m[1][2]*s,
            m[2][0]*s, m[2][1]*s, m[2][2]*s
        );
    }

    // ---------------------------------------------------------
    // Matrix-Vector Multiplication
    // ---------------------------------------------------------
    Vec3<T> operator*(const Vec3<T>& v) const {
        return Vec3<T>(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z
        );
    }

    Vec2<T> operator*(const Vec2<T>& v) const {
        // Treat as 2D point (z=1)
        return Vec2<T>(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2],
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]
        );
    }

    // ---------------------------------------------------------
    // Assignment Operators
    // ---------------------------------------------------------
    Mat3& operator+=(const Mat3& o) { *this = *this + o; return *this; }
    Mat3& operator-=(const Mat3& o) { *this = *this - o; return *this; }
    Mat3& operator*=(const Mat3& o) { *this = *this * o; return *this; }
    Mat3& operator+=(T s) { *this = *this + s; return *this; }
    Mat3& operator*=(T s) { *this = *this * s; return *this; }

    // ---------------------------------------------------------
    // Matrix Properties
    // ---------------------------------------------------------
    Mat3 transposed() const {
        return Mat3(
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2]
        );
    }

    T determinant() const {
        return m[0][0] * (m[1][1]*m[2][2] - m[1][2]*m[2][1])
             - m[0][1] * (m[1][0]*m[2][2] - m[1][2]*m[2][0])
             + m[0][2] * (m[1][0]*m[2][1] - m[1][1]*m[2][0]);
    }

    Mat3 inverse() const {
        T det = determinant();
        if (det == 0) throw std::runtime_error("Matrix is singular, cannot invert");
        T inv_det = static_cast<T>(1) / det;
        
        return Mat3(
            (m[1][1]*m[2][2] - m[1][2]*m[2][1]) * inv_det,
            -(m[0][1]*m[2][2] - m[0][2]*m[2][1]) * inv_det,
            (m[0][1]*m[1][2] - m[0][2]*m[1][1]) * inv_det,
            
            -(m[1][0]*m[2][2] - m[1][2]*m[2][0]) * inv_det,
            (m[0][0]*m[2][2] - m[0][2]*m[2][0]) * inv_det,
            -(m[0][0]*m[1][2] - m[0][2]*m[1][0]) * inv_det,
            
            (m[1][0]*m[2][1] - m[1][1]*m[2][0]) * inv_det,
            -(m[0][0]*m[2][1] - m[0][1]*m[2][0]) * inv_det,
            (m[0][0]*m[1][1] - m[0][1]*m[1][0]) * inv_det
        );
    }

    // ---------------------------------------------------------
    // Transformation Builders
    // ---------------------------------------------------------
    static Mat3 identity() { return Mat3(); }

    static Mat3 translation(T x, T y) {
        Mat3 r = identity();
        r.m[0][2] = x;
        r.m[1][2] = y;
        return r;
    }

    static Mat3 translation(const Vec2<T>& v) {
        return translation(v.x, v.y);
    }

    static Mat3 scaling(T x, T y) {
        Mat3 r = identity();
        r.m[0][0] = x;
        r.m[1][1] = y;
        return r;
    }

    static Mat3 scaling(const Vec2<T>& v) {
        return scaling(v.x, v.y);
    }

    static Mat3 rotation(T angle_rad) {
        Mat3 r = identity();
        T c = std::cos(angle_rad);
        T s = std::sin(angle_rad);
        r.m[0][0] = c; r.m[0][1] = -s;
        r.m[1][0] = s; r.m[1][1] = c;
        return r;
    }
};

// Type Aliases
using Mat3f = Mat3<float>;
using Mat3d = Mat3<double>;

} // namespace qmath
