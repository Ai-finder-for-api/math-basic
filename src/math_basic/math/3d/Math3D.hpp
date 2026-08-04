#pragma once
#include "Simd.hpp"
#include <cmath>

// =========================================================
// Vec2 Implementation
// =========================================================
struct Vec2 {
    float x, y;
    Vec2() : x(0), y(0) {}
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2 operator+(const Vec2& o) const { return Vec2(x+o.x, y+o.y); }
    Vec2 operator-(const Vec2& o) const { return Vec2(x-o.x, y-o.y); }
    Vec2 operator*(float s) const { return Vec2(x*s, y*s); }
    float dot(const Vec2& o) const { return x*o.x + y*o.y; }
    float length_sq() const { return x*x + y*y; }
    float length() const { return std::sqrt(length_sq()); }
    Vec2 normalized() const { float l = length(); return l > 0 ? Vec2(x/l, y/l) : Vec2(); }
    Vec2 lerp(const Vec2& o, float t) const { return Vec2(x+(o.x-x)*t, y+(o.y-y)*t); }
};

// =========================================================
// Vec3 Implementation
// =========================================================
struct Vec3 {
    float x, y, z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& o) const { return Vec3(x+o.x, y+o.y, z+o.z); }
    Vec3 operator-(const Vec3& o) const { return Vec3(x-o.x, y-o.y, z-o.z); }
    Vec3 operator*(float s) const { return Vec3(x*s, y*s, z*s); }
    float dot(const Vec3& o) const { return x*o.x + y*o.y + z*o.z; }
    float length_sq() const { return x*x + y*y + z*z; }
    float length() const { return std::sqrt(length_sq()); }
    Vec3 cross(const Vec3& o) const { return Vec3(y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x); }
    Vec3 normalized() const { float l = length(); return l > 0 ? Vec3(x/l, y/l, z/l) : Vec3(); }
    Vec3 lerp(const Vec3& o, float t) const { return Vec3(x+(o.x-x)*t, y+(o.y-y)*t, z+(o.z-z)*t); }
    Vec3 reflect(const Vec3& normal) const { return *this - normal * (2.0f * dot(normal)); }
};

// =========================================================
// Vec4 Implementation (SIMD Accelerated)
// =========================================================
class Vec4 {
private:
    SIMD_TYPE data;
public:
    Vec4() : data(SIMD_SETZERO) {}
    Vec4(float x, float y, float z, float w) : data(SIMD_SET(w, z, y, x)) {}
    Vec4(SIMD_TYPE v) : data(v) {}
    SIMD_TYPE get_data() const { return data; }
    Vec4 operator+(const Vec4& o) const { return Vec4(SIMD_ADD(data, o.data)); }
    Vec4 operator-(const Vec4& o) const { return Vec4(SIMD_SUB(data, o.data)); }
    Vec4 operator*(float s) const { return Vec4(SIMD_MUL(data, SIMD_SET1(s))); }
    float dot(const Vec4& o) const { return simd_hadd(SIMD_MUL(data, o.data)); }
    float length_sq() const { return simd_hadd(SIMD_MUL(data, data)); }
    float length() const { return std::sqrt(length_sq()); }
    Vec4 normalized() const { float l = length(); return l > 0 ? *this * (1.0f/l) : *this; }
    Vec4 lerp(const Vec4& o, float t) const { return *this + (o - *this) * t; }
};

// =========================================================
// Mat3 Implementation (3x3 Matrix)
// =========================================================
struct Mat3 {
    float m[9]; // Row-major
    Mat3() { for(int i=0; i<9; ++i) m[i]=0; m[0]=m[4]=m[8]=1; }
    Mat3 operator*(const Mat3& o) const {
        Mat3 r;
        for(int i=0; i<3; ++i)
            for(int j=0; j<3; ++j) {
                r.m[i*3+j] = m[i*3]*o.m[j] + m[i*3+1]*o.m[3+j] + m[i*3+2]*o.m[6+j];
            }
        return r;
    }
    Vec3 operator*(const Vec3& v) const {
        return Vec3(
            m[0]*v.x + m[1]*v.y + m[2]*v.z,
            m[3]*v.x + m[4]*v.y + m[5]*v.z,
            m[6]*v.x + m[7]*v.y + m[8]*v.z
        );
    }
    static Mat3 identity() { return Mat3(); }
    static Mat3 rotateX(float rad) {
        Mat3 r; float c=cos(rad), s=sin(rad);
        r.m[4]=c; r.m[5]=-s; r.m[7]=s; r.m[8]=c;
        return r;
    }
    static Mat3 rotateY(float rad) {
        Mat3 r; float c=cos(rad), s=sin(rad);
        r.m[0]=c; r.m[2]=s; r.m[6]=-s; r.m[8]=c;
        return r;
    }
    static Mat3 rotateZ(float rad) {
        Mat3 r; float c=cos(rad), s=sin(rad);
        r.m[0]=c; r.m[1]=-s; r.m[3]=s; r.m[4]=c;
        return r;
    }
};

// =========================================================
// Mat4 Implementation (4x4 Matrix)
// =========================================================
struct Mat4 {
    float m[16]; // Row-major
    Mat4() { for(int i=0; i<16; ++i) m[i]=0; m[0]=m[5]=m[10]=m[15]=1; }
    Mat4 operator*(const Mat4& o) const {
        Mat4 r;
        for(int i=0; i<4; ++i)
            for(int j=0; j<4; ++j) {
                r.m[i*4+j] = m[i*4]*o.m[j] + m[i*4+1]*o.m[4+j] + m[i*4+2]*o.m[8+j] + m[i*4+3]*o.m[12+j];
            }
        return r;
    }
    Vec4 operator*(const Vec4& v) const {
        SIMD_TYPE x = SIMD_SET1(v.get_data()[0]); // simplified
        return Vec4(
            m[0]*v.get_data()[0] + m[1]*v.get_data()[1] + m[2]*v.get_data()[2] + m[3]*v.get_data()[3],
            m[4]*v.get_data()[0] + m[5]*v.get_data()[1] + m[6]*v.get_data()[2] + m[7]*v.get_data()[3],
            m[8]*v.get_data()[0] + m[9]*v.get_data()[1] + m[10]*v.get_data()[2] + m[11]*v.get_data()[3],
            m[12]*v.get_data()[0] + m[13]*v.get_data()[1] + m[14]*v.get_data()[2] + m[15]*v.get_data()[3]
        );
    }
    static Mat4 identity() { return Mat4(); }
    static Mat4 translation(float x, float y, float z) {
        Mat4 r = identity(); r.m[3]=x; r.m[7]=y; r.m[11]=z; return r;
    }
    static Mat4 scale(float x, float y, float z) {
        Mat4 r = identity(); r.m[0]=x; r.m[5]=y; r.m[10]=z; return r;
    }
    static Mat4 perspective(float fovy, float aspect, float near, float far) {
        Mat4 r; float f = 1.0f / tan(fovy/2);
        r.m[0]=f/aspect; r.m[5]=f; r.m[11]=(far+near)/(near-far); r.m[14]=(2*far*near)/(near-far); r.m[15]=0;
        return r;
    }
};

// =========================================================
// Quaternion Implementation
// =========================================================
struct Quaternion {
    float w, x, y, z;
    Quaternion() : w(1), x(0), y(0), z(0) {}
    Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}
    Quaternion operator*(const Quaternion& o) const {
        return Quaternion(
            w*o.w - x*o.x - y*o.y - z*o.z,
            w*o.x + x*o.w + y*o.z - z*o.y,
            w*o.y - x*o.z + y*o.w + z*o.x,
            w*o.z + x*o.y - y*o.x + z*o.w
        );
    }
    Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }
    Vec3 rotate(const Vec3& v) const {
        Quaternion qv(0, v.x, v.y, v.z);
        Quaternion result = (*this) * qv * conjugate();
        return Vec3(result.x, result.y, result.z);
    }
    static Quaternion from_axis_angle(Vec3 axis, float angle) {
        float half = angle * 0.5f;
        float s = sin(half);
        return Quaternion(cos(half), axis.x*s, axis.y*s, axis.z*s);
    }
};
