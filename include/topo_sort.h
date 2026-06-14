/*
 * MIT License
 *
 * Copyright (c) 2026 Maksym Matskevich
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef DAGPP_TOPO_SORT_H
#define DAGPP_TOPO_SORT_H
#include <expected>
#include <queue>
#include <string>
#include <vector>
#include "usings.h"
#include "helpers.h"

namespace dagpp {
    template<directed_graph TGraph>
    constexpr std::expected<std::vector<nodeid_t>, std::string> topo_sort(const TGraph& graph) {
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
