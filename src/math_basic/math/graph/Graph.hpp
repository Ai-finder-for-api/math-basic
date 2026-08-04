/**
 * @file Graph.hpp
 * @brief Directed/Undirected Graph representation and traversal.
 */
#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

namespace qmath {

struct Edge {
    int to;
    float weight;
    Edge(int t, float w) : to(t), weight(w) {}
};

class Graph {
private:
    std::unordered_map<int, std::vector<Edge>> adj_list;

public:
    void add_node(int id) {
        if (adj_list.find(id) == adj_list.end()) {
            adj_list[id] = {};
        }
    }

    void add_edge(int from, int to, float weight, bool directed = false) {
        add_node(from);
        add_node(to);
        adj_list[from].emplace_back(to, weight);
        if (!directed) {
            adj_list[to].emplace_back(from, weight);
        }
    }

    const std::vector<Edge>& get_neighbors(int id) const {
        static std::vector<Edge> empty;
        auto it = adj_list.find(id);
        if (it == adj_list.end()) return empty;
        return it->second;
    }

    bool has_node(int id) const {
        return adj_list.find(id) != adj_list.end();
    }

    size_t node_count() const {
        return adj_list.size();
    }
};

} // namespace qmath
