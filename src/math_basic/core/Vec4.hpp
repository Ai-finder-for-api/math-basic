#pragma once
#include "Simd.hpp"

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

    py::buffer_info get_buffer_info() {
        return py::buffer_info(&data, sizeof(float), py::format_descriptor<float>::format(), 1, { 4 }, { sizeof(float) });
    }
};
