//
// Created by inquaterni on 3/17/26.
//

#ifndef DAGPP_DIRECTED_GRAPH_H
#define DAGPP_DIRECTED_GRAPH_H
#include <expected>
#include <limits>
#include <optional>
#include <queue>
#include <span>
#include <string>
#include <vector>

namespace dagpp {
    template<typename T>
    concept node = std::is_trivial_v<T>;
    template<node TNode>
    class digraph_builder;
    template<node TNode, typename ...TExtension>
    class digraph;

    using nodeid_t = std::size_t;
    using edgeid_t = std::size_t;


    template<node TNode>
    class digraph_builder {
    public:
        constexpr nodeid_t add_node(TNode node);
        constexpr void add_edge(nodeid_t from, nodeid_t to);
        constexpr void reserve_nodes(std::size_t n);
        constexpr void reserve_edges(std::size_t n);
        template<typename ...TExtension>
        [[nodiscard]]
        constexpr digraph<TNode, TExtension...> compile();
    private:
        struct edge_t { nodeid_t from, to; };
        std::vector<TNode> m_nodes;
        std::vector<edge_t> m_edges;
    };

    template<node TNode, typename ...TExtension>
    class digraph: public TExtension... {
    public:
        using node_t = TNode;
        [[nodiscard]]
        constexpr const node_t& node(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::size_t count() const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> out_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> in_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr bool is_acyclic() const;

    private:
        friend class digraph_builder<node_t>;
        std::vector<node_t> m_nodes;
        std::vector<edgeid_t> m_offsets;
        std::vector<nodeid_t> m_edges;
        std::vector<edgeid_t> m_rev_offsets;
        std::vector<nodeid_t> m_rev_edges;
    };

    template<node TNode>
    template<typename ...TExtension>
    constexpr digraph<TNode, TExtension...> digraph_builder<TNode>::compile() {
        digraph<TNode, TExtension...> result;
        const auto V = m_nodes.size();
        const auto E = m_edges.size();

        result.m_nodes = std::move(m_nodes);

        result.m_offsets.assign(V + 1, 0);
        result.m_edges.resize(E);
        for (const auto& e : m_edges) {
            ++result.m_offsets[e.from + 1];
        }
        for (std::size_t i = 1; i <= V; ++i) {
            result.m_offsets[i] += result.m_offsets[i - 1];
        }
        auto fwd_offsets = result.m_offsets;
        for (const auto& e : m_edges) {
            result.m_edges[fwd_offsets[e.from]++] = e.to;
        }


        result.m_rev_offsets.assign(V + 1, 0);
        result.m_rev_edges.resize(E);
        for (const auto& e : m_edges) {
            ++result.m_rev_offsets[e.to + 1];
        }
        for (std::size_t i = 1; i <= V; ++i) {
            result.m_rev_offsets[i] += result.m_rev_offsets[i - 1];
        }
        auto rev_offsets = result.m_rev_offsets;
        for (const auto& e : m_edges) {
            result.m_rev_edges[rev_offsets[e.to]++] = e.from;
        }

        m_edges.clear();
        return result;
    }

    template<node TNode>
    constexpr nodeid_t digraph_builder<TNode>::add_node(TNode node) {
        m_nodes.emplace_back(node);
        return m_nodes.size() - 1;
    }

    template<node TNode>
    constexpr void digraph_builder<TNode>::add_edge(nodeid_t from, nodeid_t to) {
        m_edges.emplace_back(edge_t {from, to});
    }

    template<node TNode>
    constexpr void digraph_builder<TNode>::reserve_nodes(std::size_t n) {
        m_nodes.reserve(n);
    }

    template<node TNode>
    constexpr void digraph_builder<TNode>::reserve_edges(std::size_t n) {
        m_edges.reserve(n);
    }


    template<node TNode, typename... TExtension>
    constexpr const TNode & digraph<TNode, TExtension...>::node(nodeid_t id) const {
        return m_nodes[id];
    }

    template<node TNode, typename... TExtension>
    constexpr std::size_t digraph<TNode, TExtension...>::count() const {
        return m_nodes.size();
    }

    template<node TNode, typename ... TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string> digraph<TNode, TExtension...>::out_edges(const nodeid_t id) const {
        if (m_offsets.size() - 1 <= id) {
            return std::unexpected{"Index is out of range."};
        }
        const auto start = m_offsets[id];
        const auto end = m_offsets[id + 1];
        return std::span {m_edges.begin() + start, end - start};
    }

    template<node TNode, typename ... TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string> digraph<TNode, TExtension...>::in_edges(const nodeid_t id) const {
        if (m_rev_offsets.size() - 1 <= id) {
            return std::unexpected{"Index is out of range."};
        }
        const auto start = m_rev_offsets[id];
        const auto end = m_rev_offsets[id + 1];
        return std::span {m_rev_edges.begin() + start, end - start};
    }

    template<node TNode, typename ... TExtension>
    constexpr bool digraph<TNode, TExtension...>::is_acyclic() const {
        const auto n = count();
        if (n == 0) return true;

        std::vector<std::size_t> in_degree(n, 0);
        for (nodeid_t u = 0; u < n; ++u) {
            auto edges = out_edges(u);
            if (!edges) continue;
            for (const auto v : *edges) {
                ++in_degree[v];
            }
        }

        std::size_t processed = 0;
        std::queue<nodeid_t> queue;
        for (nodeid_t u = 0; u < n; ++u) {
            if (in_degree[u] == 0) queue.push(u);
        }
        while (!queue.empty()) {
            const auto u = queue.front();
            queue.pop();
            ++processed;
            auto edges = out_edges(u);
            if (!edges) continue;
            for (const auto v : *edges) {
                if (--in_degree[v] == 0) queue.push(v);
            }
        }
        return processed == n;
    }
} // dagpp

#endif //DAGPP_DIRECTED_GRAPH_H