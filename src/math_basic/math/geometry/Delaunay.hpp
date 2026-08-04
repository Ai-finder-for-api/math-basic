/**
 * @file Delaunay.hpp
 * @brief 2D Delaunay Triangulation using Bowyer-Watson algorithm.
 * 
 * Generates a mesh of triangles where no point is inside the circumcircle
 * of any triangle. Crucial for terrain generation and finite element analysis.
 */
#pragma once
#include "ConvexHull.hpp"
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

namespace qmath {

template<typename T>
struct DelaunayTriangle {
    Vec2<T> a, b, c;
    DelaunayTriangle(const Vec2<T>& a, const Vec2<T>& b, const Vec2<T>& c) : a(a), b(b), c(c) {}
    
    bool in_circumcircle(const Vec2<T>& p) const {
        T ax = a.x - p.x, ay = a.y - p.y;
        T bx = b.x - p.x, by = b.y - p.y;
        T cx = c.x - p.x, cy = c.y - p.y;
        
        T det = (ax * ax + ay * ay) * (bx * cy - cx * by) -
                (bx * bx + by * by) * (ax * cy - cx * ay) +
                (cx * cx + cy * cy) * (ax * by - bx * ay);
                
        return det > 0;
    }
};

template<typename T>
class DelaunayTriangulation {
public:
    std::vector<DelaunayTriangle<T>> triangulate(std::vector<Vec2<T>> points) {
        std::vector<DelaunayTriangle<T>> triangles;
        if (points.size() < 3) return triangles;

        // Create super-triangle large enough to contain all points
        T min_x = points[0].x, max_x = points[0].x;
        T min_y = points[0].y, max_y = points[0].y;
        for (const auto& p : points) {
            min_x = std::min(min_x, p.x);
            max_x = std::max(max_x, p.x);
            min_y = std::min(min_y, p.y);
            max_y = std::max(max_y, p.y);
        }
        
        T dx = max_x - min_x;
        T dy = max_y - min_y;
        T delta_max = std::max(dx, dy);
        T mid_x = (min_x + max_x) / 2;
        T mid_y = (min_y + max_y) / 2;
        
        Vec2<T> p1(mid_x - 20 * delta_max, mid_y - delta_max);
        Vec2<T> p2(mid_x, mid_y + 20 * delta_max);
        Vec2<T> p3(mid_x + 20 * delta_max, mid_y - delta_max);
        
        triangles.push_back(DelaunayTriangle<T>(p1, p2, p3));

        for (const auto& p : points) {
            std::vector<DelaunayTriangle<T>> bad_triangles;
            
            for (const auto& tri : triangles) {
                if (tri.in_circumcircle(p)) {
                    bad_triangles.push_back(tri);
                }
            }
            
            std::vector<std::pair<Vec2<T>, Vec2<T>>> polygon;
            
            for (const auto& tri : bad_triangles) {
                auto edges = {
                    std::make_pair(tri.a, tri.b),
                    std::make_pair(tri.b, tri.c),
                    std::make_pair(tri.c, tri.a)
                };
                
                for (const auto& edge : edges) {
                    bool is_shared = false;
                    for (const auto& other_tri : bad_triangles) {
                        if (&tri == &other_tri) continue;
                        
                        auto other_edges = {
                            std::make_pair(other_tri.a, other_tri.b),
                            std::make_pair(other_tri.b, other_tri.c),
                            std::make_pair(other_tri.c, other_tri.a)
                        };
                        
                        for (const auto& other_edge : other_edges) {
                            if ((edge.first == other_edge.first && edge.second == other_edge.second) ||
                                (edge.first == other_edge.second && edge.second == other_edge.first)) {
                                is_shared = true;
                                break;
                            }
                        }
                        if (is_shared) break;
                    }
                    if (!is_shared) polygon.push_back(edge);
                }
            }
            
            // Remove bad triangles
            triangles.erase(std::remove_if(triangles.begin(), triangles.end(), 
                [&bad_triangles](const DelaunayTriangle<T>& t) {
                    return std::find(bad_triangles.begin(), bad_triangles.end(), t) != bad_triangles.end();
                }), triangles.end());
            
            // Add new triangles connecting p to polygon edges
            for (const auto& edge : polygon) {
                triangles.push_back(DelaunayTriangle<T>(edge.first, edge.second, p));
            }
        }
        
        // Remove triangles that share a vertex with the super-triangle
        triangles.erase(std::remove_if(triangles.begin(), triangles.end(), 
            [p1, p2, p3](const DelaunayTriangle<T>& t) {
                return t.a == p1 || t.a == p2 || t.a == p3 ||
                       t.b == p1 || t.b == p2 || t.b == p3 ||
                       t.c == p1 || t.c == p2 || t.c == p3;
            }), triangles.end());
            
        return triangles;
    }
};

} // namespace qmath
