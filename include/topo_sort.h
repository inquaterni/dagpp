//
// Created by inquaterni on 3/17/26.
//

#ifndef DAGPP_TOPO_SORT_H
#define DAGPP_TOPO_SORT_H
#include <expected>
#include <queue>
#include <string>
#include <vector>

#include "digraph.h"

namespace dagpp {
    template<node TNode, typename ...TExtension>
    constexpr std::expected<std::vector<nodeid_t>, std::string> topo_sort(const digraph<TNode, TExtension...>& graph) {
        const auto n = graph.count();
        if (n == 0) return std::vector<nodeid_t>{};

        std::vector<std::size_t> in_degree(n, 0);
        for (nodeid_t u = 0; u < n; ++u) {
            auto edges = graph.out_edges(u);
            if (!edges) continue;
            for (const auto v : *edges) {
                ++in_degree[v];
            }
        }

        std::queue<nodeid_t> queue;
        for (nodeid_t u = 0; u < n; ++u) {
            if (in_degree[u] == 0) {
                queue.push(u);
            }
        }

        std::vector<nodeid_t> order;
        order.reserve(n);
        while (!queue.empty()) {
            const auto u = queue.front();
            queue.pop();
            order.push_back(u);

            auto edges = graph.out_edges(u);
            if (!edges) continue;
            for (const auto v : *edges) {
                if (--in_degree[v] == 0) {
                    queue.push(v);
                }
            }
        }

        if (order.size() != n) {
            return std::unexpected{"Graph contains a cycle."};
        }
        return order;
    }
} // dagpp

#endif //DAGPP_TOPO_SORT_H
