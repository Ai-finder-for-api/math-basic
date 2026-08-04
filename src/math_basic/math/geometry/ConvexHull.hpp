/**
 * @file ConvexHull.hpp
 * @brief 2D Convex Hull generation using Graham Scan.
 * 
 * Computes the smallest convex polygon that contains a set of points.
 */
#pragma once
#include "../3d/Vec2.hpp"
#include <vector>
#include <algorithm>

namespace qmath {

template<typename T>
class ConvexHull2D {
private:
    Vec2<T> pivot;

    static T cross_product(const Vec2<T>& a, const Vec2<T>& b, const Vec2<T>& c) {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    struct CompareAngle {
        const Vec2<T>& pivot;
        CompareAngle(const Vec2<T>& p) : pivot(p) {}

        bool operator()(const Vec2<T>& a, const Vec2<T>& b) const {
            T cross = cross_product(pivot, a, b);
            if (cross == 0) {
                return pivot.distance_sq(a) < pivot.distance_sq(b);
            }
            return cross > 0; // Counter-clockwise
        }
    };

public:
    std::vector<Vec2<T>> compute(std::vector<Vec2<T>> points) {
        if (points.size() < 3) return points;

        // Find lowest point (and leftmost if tied)
        size_t min_idx = 0;
        for (size_t i = 1; i < points.size(); ++i) {
            if (points[i].y < points[min_idx].y || 
                (points[i].y == points[min_idx].y && points[i].x < points[min_idx].x)) {
                min_idx = i;
            }
        }
        std::swap(points[0], points[min_idx]);
        pivot = points[0];

        // Sort points by polar angle relative to pivot
        std::sort(points.begin() + 1, points.end(), CompareAngle(pivot));

        // Remove collinear points
        std::vector<Vec2<T>> filtered;
        filtered.push_back(points[0]);
        for (size_t i = 1; i < points.size(); ++i) {
            while (i < points.size() - 1 && cross_product(pivot, points[i], points[i+1]) == 0) {
                i++;
            }
            if (i < points.size()) filtered.push_back(points[i]);
        }

        if (filtered.size() < 3) return filtered;

        // Graham Scan
        std::vector<Vec2<T>> hull;
        hull.push_back(filtered[0]);
        hull.push_back(filtered[1]);
        hull.push_back(filtered[2]);

        for (size_t i = 3; i < filtered.size(); ++i) {
            while (hull.size() > 1 && cross_product(hull[hull.size() - 2], hull[hull.size() - 1], filtered[i]) <= 0) {
                hull.pop_back();
            }
            hull.push_back(filtered[i]);
        }

        return hull;
    }
};

} // namespace qmath
