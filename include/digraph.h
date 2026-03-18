//
// Created by inquaterni on 3/17/26.
//

#ifndef DAGPP_DIRECTED_GRAPH_H
#define DAGPP_DIRECTED_GRAPH_H
#include <vector>
#include <expected>
#include <span>
#include <string>

#include "usings.h"
#include "helpers.h"

namespace dagpp {
    template <typename TNode, typename ...TExtension>
    class digraph: public TExtension... {
    public:
        using node_type = TNode;
        using size_type = std::size_t;

        constexpr nodeid_t add_node(const TNode &node);
        constexpr nodeid_t add_node(TNode &&node);
        constexpr void add_edge(nodeid_t from, nodeid_t to);
        constexpr void reserve_nodes(size_type n);

        [[nodiscard]]
        constexpr const node_type& node(nodeid_t id) const { return m_nodes[id]; }
        [[nodiscard]]
        constexpr node_type& node(nodeid_t id) { return m_nodes[id]; }
        [[nodiscard]]
        constexpr size_type count() const { return m_nodes.size(); }

        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> out_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> in_edges(nodeid_t id) const;

        [[nodiscard]]
        constexpr bool is_acyclic() const;

    private:
        std::vector<node_type> m_nodes;
        std::vector<std::vector<nodeid_t>> m_out_edges;
        std::vector<std::vector<nodeid_t>> m_in_edges;
    };

    template<typename TNode, typename ... TExtension>
    constexpr nodeid_t digraph<TNode, TExtension...>::add_node(const TNode &node) {
        m_nodes.emplace_back(node);
        m_out_edges.emplace_back();
        m_in_edges.emplace_back();
        return m_nodes.size() - 1;
    }

    template<typename TNode, typename ... TExtension>
    constexpr nodeid_t digraph<TNode, TExtension...>::add_node(TNode &&node) {
        m_nodes.emplace_back(std::move(node));
        m_out_edges.emplace_back();
        m_in_edges.emplace_back();
        return m_nodes.size() - 1;
    }

    template<typename TNode, typename ... TExtension>
    constexpr void digraph<TNode, TExtension...>::add_edge(const nodeid_t from, const nodeid_t to) {
        m_out_edges[from].emplace_back(to);
        m_in_edges[to].emplace_back(from);
    }

    template<typename TNode, typename ... TExtension>
    constexpr void digraph<TNode, TExtension...>::reserve_nodes(size_type n) {
        m_nodes.reserve(n);
        m_out_edges.reserve(n);
        m_in_edges.reserve(n);
    }

    template<typename TNode, typename ... TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string> digraph<TNode, TExtension...>::out_edges(
    const nodeid_t id) const {
        if (id >= m_out_edges.size()) return std::unexpected("Index is out of range.");
        return m_out_edges[id];
    }

    template<typename TNode, typename ... TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string> digraph<TNode, TExtension...>::in_edges(
    const nodeid_t id) const {
        if (id >= m_in_edges.size()) return std::unexpected("Index is out of range.");
        return m_in_edges[id];
    }

    template<typename TNode, typename ... TExtension>
    constexpr bool digraph<TNode, TExtension...>::is_acyclic() const {
        const auto n = count();
        if (n == 0) return true;

        std::vector<size_type> in_degree(n);
        std::vector<nodeid_t> queue;
        queue.reserve(n);

        for (nodeid_t u = 0; u < n; ++u) {
            in_degree[u] = m_in_edges[u].size();
            if (in_degree[u] == 0) queue.emplace_back(u);
        }

        size_type head = 0;
        while (head < queue.size()) {
            for (const auto u = queue[head++]; const auto v : m_out_edges[u]) {
                if (--in_degree[v] == 0) queue.emplace_back(v);
            }
        }
        return queue.size() == n;
    }

    struct _static_test_node {
        int id;
    };
    static_assert(directed_graph<digraph<_static_test_node>>,
        "Mutable digraph does not satisfy `directed_graph` concept");
} // dagpp

#endif //DAGPP_DIRECTED_GRAPH_H