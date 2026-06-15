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

#ifndef DAGPP_DIJKSTRA_H
#define DAGPP_DIJKSTRA_H

#include <vector>
#include <limits>
#include <algorithm>
#include <ranges>
#include "helpers.h"
#include "usings.h"

namespace dagpp {

    template<number TWeight>
    struct dijkstra_result {
        std::vector<TWeight> distances;
        std::vector<nodeid_t> previous;
    };

    template <wdirected_graph TGraph>
    constexpr dijkstra_result<typename TGraph::weight_type> dijkstra(const TGraph& g, nodeid_t source) {
        using weight_type = TGraph::weight_type;
        const auto n = g.node_count();
        
        constexpr auto inf = std::numeric_limits<weight_type>::max();
        constexpr auto no_prev = std::numeric_limits<nodeid_t>::max();
        
        dijkstra_result<weight_type> result;
        result.distances.assign(n, inf);
        result.previous.assign(n, no_prev);
        
        if (source >= n) {
            return result;
        }
        
        result.distances[source] = 0;
        
        struct pq_element {
            weight_type dist;
            nodeid_t node;
            bool operator >(const pq_element& other) const {
                return dist > other.dist;
            }
        };
        
        std::vector<pq_element> pq;
        pq.push_back(pq_element{0, source});
        
        while (!pq.empty()) {
            std::pop_heap(pq.begin(), pq.end(), std::greater<pq_element>{});
            auto [d, u] = pq.back();
            pq.pop_back();
            
            if (d > result.distances[u]) continue;
            
            auto edges_res = g.out_edges(u);
            auto weights_res = g.out_weights(u);
            
            if (edges_res && weights_res) {
                for (auto [v, weight] : std::views::zip(*edges_res, *weights_res)) {
                    if (result.distances[u] + weight < result.distances[v]) {
                        result.distances[v] = result.distances[u] + weight;
                        result.previous[v] = u;
                        pq.push_back(pq_element{result.distances[v], v});
                        std::push_heap(pq.begin(), pq.end(), std::greater<pq_element>{});
                    }
                }
            }
        }
        
        return result;
    }

} // namespace dagpp

#endif //DAGPP_DIJKSTRA_H