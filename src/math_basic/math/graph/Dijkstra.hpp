/**
 * @file Dijkstra.hpp
 * @brief Dijkstra's Shortest Path Algorithm.
 * 
 * Finds the lowest-cost path between two nodes in a graph with non-negative weights.
 */
#pragma once
#include "Graph.hpp"
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

namespace qmath {

struct PathResult {
    std::vector<int> path;
    float total_cost;
    bool path_found;
};

inline PathResult dijkstra(const Graph& graph, int start, int end) {
    if (!graph.has_node(start) || !graph.has_node(end)) return { {}, 0.0f, false };

    std::unordered_map<int, float> distances;
    std::unordered_map<int, int> previous;
    
    // Priority queue stores pairs of (cost, node_id)
    using pq_element = std::pair<float, int>;
    std::priority_queue<pq_element, std::vector<pq_element>, std::greater<pq_element>> pq;

    // Initialize distances to infinity
    // We only add nodes to the queue as we discover them to save memory.
    distances[start] = 0.0f;
    pq.push({0.0f, start});

    while (!pq.empty()) {
        auto [current_dist, current_node] = pq.top();
        pq.pop();

        // If we reached the destination, we can stop
        if (current_node == end) break;

        // Optimization: skip if we already found a better path
        if (current_dist > distances[current_node]) continue;

        for (const auto& edge : graph.get_neighbors(current_node)) {
            float new_dist = current_dist + edge.weight;
            
            // If this node hasn't been visited, or we found a shorter path
            if (distances.find(edge.to) == distances.end() || new_dist < distances[edge.to]) {
                distances[edge.to] = new_dist;
                previous[edge.to] = current_node;
                pq.push({new_dist, edge.to});
            }
        }
    }

    // Reconstruct path
    if (distances.find(end) == distances.end()) {
        return { {}, 0.0f, false }; // No path exists
    }

    std::vector<int> path;
    int current = end;
    while (current != start) {
        path.push_back(current);
        current = previous[current];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return { path, distances[end], true };
}

} // namespace qmath
