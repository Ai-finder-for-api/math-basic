/**
 * @file BVH.hpp
 * @brief Bounding Volume Hierarchy for fast Raytracing intersection.
 * 
 * A binary tree of Axis-Aligned Bounding Boxes (AABBs) that allows
 * O(log(N)) ray-mesh intersection tests instead of O(N).
 */
#pragma once
#include "../3d/AABB.hpp"
#include "../3d/Ray.hpp"
#include "RayTriangle.hpp"
#include <vector>
#include <memory>
#include <algorithm>

namespace qmath {

template<typename T>
struct BVHNode {
    AABB<T> bounds;
    std::unique_ptr<BVHNode<T>> left;
    std::unique_ptr<BVHNode<T>> right;
    std::vector<size_t> triangle_indices; // Only populated for leaf nodes

    bool is_leaf() const { return !left && !right; }
};

template<typename T>
class BVH {
private:
    std::unique_ptr<BVHNode<T>> root;
    const std::vector<Triangle<T>>* triangles_ptr;

    // Build node recursively
    std::unique_ptr<BVHNode<T>> build_node(std::vector<size_t> indices, int depth) {
        auto node = std::make_unique<BVHNode<T>>();
        
        if (indices.empty()) return node;

        // Compute bounds for all triangles in this node
        AABB<T> bounds((*triangles_ptr)[indices[0]].v0, (*triangles_ptr)[indices[0]].v0);
        for (size_t idx : indices) {
            const auto& tri = (*triangles_ptr)[idx];
            bounds.encapsulate(tri.v0);
            bounds.encapsulate(tri.v1);
            bounds.encapsulate(tri.v2);
        }
        node->bounds = bounds;

        // If few enough triangles, make a leaf
        if (indices.size() <= 4 || depth > 20) {
            node->triangle_indices = std::move(indices);
            return node;
        }

        // Find longest axis to split along
        Vec3<T> extents = bounds.extents();
        int axis = 0;
        if (extents.y > extents.x) axis = 1;
        if (extents.z > extents.y) axis = 2;

        // Sort triangles along the longest axis based on centroid
        std::sort(indices.begin(), indices.end(), [this, axis](size_t a, size_t b) {
            const auto& tri_a = (*triangles_ptr)[a];
            const auto& tri_b = (*triangles_ptr)[b];
            Vec3<T> c_a = (tri_a.v0 + tri_a.v1 + tri_a.v2) / static_cast<T>(3);
            Vec3<T> c_b = (tri_b.v0 + tri_b.v1 + tri_b.v2) / static_cast<T>(3);
            return c_a[axis] < c_b[axis];
        });

        size_t mid = indices.size() / 2;
        std::vector<size_t> left_indices(indices.begin(), indices.begin() + mid);
        std::vector<size_t> right_indices(indices.begin() + mid, indices.end());

        node->left = build_node(std::move(left_indices), depth + 1);
        node->right = build_node(std::move(right_indices), depth + 1);

        return node;
    }

    // Traverse node recursively
    void traverse_node(const BVHNode<T>* node, const Ray<T>& ray, T& closest_t, size_t& hit_tri_idx) const {
        if (!node) return;
        
        T t = node->bounds.intersect_ray(ray);
        if (t < 0 || t > closest_t) return; // Missed bounds, or farther than current best

        if (node->is_leaf()) {
            for (size_t idx : node->triangle_indices) {
                T tri_t = (*triangles_ptr)[idx].intersect_ray(ray);
                if (tri_t > 0 && tri_t < closest_t) {
                    closest_t = tri_t;
                    hit_tri_idx = idx;
                }
            }
        } else {
            traverse_node(node->left.get(), ray, closest_t, hit_tri_idx);
            traverse_node(node->right.get(), ray, closest_t, hit_tri_idx);
        }
    }

public:
    void build(const std::vector<Triangle<T>>& triangles) {
        triangles_ptr = &triangles;
        std::vector<size_t> indices(triangles.size());
        for (size_t i = 0; i < triangles.size(); ++i) indices[i] = i;
        root = build_node(std::move(indices), 0);
    }

    // Returns distance t and index of hit triangle. Returns -1 if missed.
    std::pair<T, size_t> intersect(const Ray<T>& ray) const {
        T closest_t = std::numeric_limits<T>::max();
        size_t hit_tri_idx = -1;
        traverse_node(root.get(), ray, closest_t, hit_tri_idx);
        if (hit_tri_idx == -1) return { -1, -1 };
        return { closest_t, hit_tri_idx };
    }
};

} // namespace qmath
