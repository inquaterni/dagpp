//
// Created by inquaterni on 3/21/26.
//

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
        const auto n = g.count();
        
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