/**
 * @file AStar.hpp
 * @brief A* Pathfinding Algorithm.
 * 
 * More efficient than Dijkstra when a heuristic (e.g., Euclidean distance)
 * is available to guide the search.
 */
#pragma once
#include <utility>
#include "Graph.hpp"
#include "Dijkstra.hpp"
#include "../3d/Vec3.hpp"
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>
#include <algorithm>
#include <cmath>

namespace qmath {

// Heuristic function type: takes node_id, returns estimated cost to goal
using HeuristicFunc = std::function<float(int)>;

inline PathResult astar(const Graph& graph, int start, int end, HeuristicFunc heuristic) {
    if (!graph.has_node(start) || !graph.has_node(end)) return { {}, 0.0f, false };

    std::unordered_map<int, float> g_cost; // Cost from start to node
    std::unordered_map<int, float> f_cost; // Estimated total cost (g + h)
    std::unordered_map<int, int> previous;
    
    using pq_element = std::pair<float, int>; // (f_cost, node_id)
    std::priority_queue<pq_element, std::vector<pq_element>, std::greater<pq_element>> open_set;

    g_cost[start] = 0.0f;
    f_cost[start] = heuristic(start);
    open_set.push({f_cost[start], start});

    while (!open_set.empty()) {
        auto [current_f, current_node] = open_set.top();
        open_set.pop();

        if (current_node == end) break;

        // Optimization: skip if we found a better path to this node already
        if (current_f > f_cost[current_node]) continue;

        for (const auto& edge : graph.get_neighbors(current_node)) {
            float tentative_g = g_cost[current_node] + edge.weight;
            
            if (g_cost.find(edge.to) == g_cost.end() || tentative_g < g_cost[edge.to]) {
                g_cost[edge.to] = tentative_g;
                float h = heuristic(edge.to);
                f_cost[edge.to] = tentative_g + h;
                previous[edge.to] = current_node;
                open_set.push({f_cost[edge.to], edge.to});
            }
        }
    }

    // Reconstruct path
    if (g_cost.find(end) == g_cost.end()) {
        return { {}, 0.0f, false };
    }

    std::vector<int> path;
    int current = end;
    while (current != start) {
        path.push_back(current);
        if (previous.find(current) == previous.end()) return { {}, 0.0f, false }; // Path broken
        current = previous[current];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return { path, g_cost[end], true };
}

} // namespace qmath
