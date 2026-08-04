/**
 * @file Splines.hpp
 * @brief Advanced Spline interpolations (B-Splines, Catmull-Rom, NURBS).
 * 
 * Used in CAD, camera path animations, and smooth curve generation.
 */
#pragma once
#include "Vec3.hpp"
#include <vector>
#include <cmath>

namespace qmath {

template<typename T>
class CatmullRomSpline {
private:
    std::vector<Vec3<T>> control_points;
public:
    CatmullRomSpline() {}
    void add_point(const Vec3<T>& p) { control_points.push_back(p); }
    
    Vec3<T> evaluate(T t) const {
        if (control_points.size() < 4) return Vec3<T>();
        
        // t is in [0, 1] across the whole spline
        T scaled_t = t * (control_points.size() - 3);
        int segment = static_cast<int>(scaled_t);
        if (segment >= static_cast<int>(control_points.size() - 3)) segment = control_points.size() - 4;
        
        T local_t = scaled_t - segment;
        
        const Vec3<T>& p0 = control_points[segment];
        const Vec3<T>& p1 = control_points[segment + 1];
        const Vec3<T>& p2 = control_points[segment + 2];
        const Vec3<T>& p3 = control_points[segment + 3];
        
        T t2 = local_t * local_t;
        T t3 = t2 * local_t;
        
        return 0.5f * (
            (2.0f * p1) +
            (-p0 + p2) * local_t +
            (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
            (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3
        );
    }
};

template<typename T>
class BSpline {
private:
    std::vector<Vec3<T>> control_points;
    std::vector<T> knots;
    int degree;

    // Cox-de Boor recursion formula for basis functions
    T basis(int i, int p, T t) const {
        if (p == 0) {
            return (knots[i] <= t && t < knots[i + 1]) ? 1.0f : 0.0f;
        }
        T c1 = 0, c2 = 0;
        T den1 = knots[i + p] - knots[i];
        if (den1 != 0) c1 = (t - knots[i]) / den1 * basis(i, p - 1, t);
        T den2 = knots[i + p + 1] - knots[i + 1];
        if (den2 != 0) c2 = (knots[i + p + 1] - t) / den2 * basis(i + 1, p - 1, t);
        return c1 + c2;
    }

public:
    BSpline(int deg = 3) : degree(deg) {}

    void add_point(const Vec3<T>& p) { 
        control_points.push_back(p); 
        update_knots(); 
    }

    void update_knots() {
        knots.clear();
        int m = control_points.size() + degree + 1;
        knots.resize(m, 0);
        for (size_t i = m - degree - 1; i < m; ++i) knots[i] = 1.0f;
        for (int i = degree + 1; i < m - degree - 1; ++i) {
            knots[i] = static_cast<T>(i - degree) / (m - 2 * degree - 1);
        }
    }

    Vec3<T> evaluate(T t) const {
        if (control_points.size() <= static_cast<size_t>(degree)) return Vec3<T>();
        Vec3<T> result(0, 0, 0);
        for (size_t i = 0; i < control_points.size(); ++i) {
            T b = basis(i, degree, t);
            result += control_points[i] * b;
        }
        return result;
    }
};

} // namespace qmath
