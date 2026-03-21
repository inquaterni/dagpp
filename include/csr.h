//
// Created by inquaterni on 3/18/26.
//

#ifndef DAGPP_CSR_H
#define DAGPP_CSR_H
#include <concepts>
#include <vector>
#include <queue>
#include <expected>
#include "usings.h"
#include "helpers.h"

namespace dagpp::csr {
    template<typename T>
    concept node = std::semiregular<T>;

    template<node TNode>
    class digraph_builder;
    template<node TNode, typename ...TExtension>
    class digraph;
    template<node TNode, number TWeight>
    class wdigraph_builder;
    template<node TNode, number TWeight, typename ...TExtension>
    class wdigraph;

    template<node TNode>
    class digraph_builder {
    public:
        constexpr nodeid_t add_node(const TNode &node);
        constexpr nodeid_t add_node(TNode &&node);
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
        using node_type = TNode;
        using size_type = std::size_t;
        [[nodiscard]]
        constexpr const node_type& node(nodeid_t id) const;
        [[nodiscard]]
        constexpr size_type count() const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> out_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> in_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr bool is_acyclic() const;
    protected:
        friend class digraph_builder<node_type>;
        std::vector<node_type> m_nodes;
        std::vector<edgeid_t> m_offsets;
        std::vector<nodeid_t> m_edges;
        std::vector<edgeid_t> m_rev_offsets;
        std::vector<nodeid_t> m_rev_edges;
    };

    template<node TNode, number TWeight>
    class wdigraph_builder {
    public:
        constexpr nodeid_t add_node(const TNode &node);
        constexpr nodeid_t add_node(TNode &&node);
        constexpr void add_edge(nodeid_t from, nodeid_t to, TWeight weight = 1);
        constexpr void reserve_nodes(std::size_t n);
        constexpr void reserve_edges(std::size_t n);
        template<typename ...TExtension>
        [[nodiscard]]
        constexpr wdigraph<TNode, TWeight, TExtension...> compile();
    private:
        struct edge_t {
            nodeid_t from, to;
            TWeight weight;
        };
        std::vector<TNode> m_nodes;
        std::vector<edge_t> m_edges;
    };

    template<node TNode, number TWeight, typename ...TExtension>
    class wdigraph: public TExtension... {
    public:
        using node_type = TNode;
        using size_type = std::size_t;
        using weight_type = TWeight;
        [[nodiscard]]
        constexpr const node_type& node(nodeid_t id) const;
        [[nodiscard]]
        constexpr size_type count() const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> out_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> in_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const weight_type>, std::string> out_weights(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const weight_type>, std::string> in_weights(nodeid_t id) const;
        [[nodiscard]]
        constexpr bool is_acyclic() const;
    protected:
        friend class digraph_builder<node_type>;
        std::vector<node_type> m_nodes;
        std::vector<edgeid_t> m_offsets;
        std::vector<nodeid_t> m_edges;
        std::vector<weight_type> m_weights;
        std::vector<edgeid_t> m_rev_offsets;
        std::vector<nodeid_t> m_rev_edges;
        std::vector<weight_type> m_rev_weights;
    };

    template<node TNode>
    template<typename ...TExtension>
    constexpr digraph<TNode, TExtension...> digraph_builder<TNode>::compile() {
        using size_type = digraph<TNode, TExtension...>::size_type;
        digraph<TNode, TExtension...> result;
        const auto V = m_nodes.size();
        const auto E = m_edges.size();

        result.m_nodes = std::move(m_nodes);

        result.m_offsets.assign(V + 1, 0);
        result.m_edges.resize(E);
        for (const auto& e : m_edges) {
            ++result.m_offsets[e.from + 1];
        }
        for (size_type i = 1; i <= V; ++i) {
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
        for (size_type i = 1; i <= V; ++i) {
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
    constexpr nodeid_t digraph_builder<TNode>::add_node(const TNode &node) {
        m_nodes.emplace_back(node);
        return m_nodes.size() - 1;
    }

    template<node TNode>
    constexpr nodeid_t digraph_builder<TNode>::add_node(TNode &&node) {
        m_nodes.emplace_back(std::forward<TNode>(node));
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
    constexpr const TNode &digraph<TNode, TExtension...>::node(nodeid_t id) const {
        if (m_nodes.size() <= id) {
            return std::unexpected{"Index is out of range."};
        }

        return m_nodes[id];
    }

    template<node TNode, typename... TExtension>
    constexpr digraph<TNode, TExtension...>::size_type digraph<TNode, TExtension...>::count() const {
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

        std::vector<size_type> in_degree(n);
        std::vector<nodeid_t> queue;
        queue.reserve(n);

        for (nodeid_t u = 0; u < n; ++u) {
            in_degree[u] = m_rev_offsets[u + 1] - m_rev_offsets[u];
            if (in_degree[u] == 0) {
                queue.emplace_back(u);
            }
        }
        size_type head = 0;
        while (head < queue.size()) {
            const auto u = queue[head++];

            if (auto edges = out_edges(u)) {
                for (const auto v : *edges) {
                    if (--in_degree[v] == 0) {
                        queue.emplace_back(v);
                    }
                }
            }
        }
        return queue.size() == n;
    }

    template<node TNode, number TWeight>
    constexpr nodeid_t wdigraph_builder<TNode, TWeight>::add_node(const TNode &node) {
        m_nodes.emplace_back(node);
        return m_nodes.size() - 1;
    }

    template<node TNode, number TWeight>
    constexpr nodeid_t wdigraph_builder<TNode, TWeight>::add_node(TNode &&node) {
        m_nodes.emplace_back(std::forward<TNode>(node));
        return m_nodes.size() - 1;
    }

    template<node TNode, number TWeight>
    constexpr void wdigraph_builder<TNode, TWeight>::add_edge(nodeid_t from, nodeid_t to, TWeight weight) {
        m_edges.emplace_back(edge_t {from, to, weight});
    }

    template<node TNode, number TWeight>
    constexpr void wdigraph_builder<TNode, TWeight>::reserve_nodes(std::size_t n) {
        m_nodes.reserve(n);
    }

    template<node TNode, number TWeight>
    constexpr void wdigraph_builder<TNode, TWeight>::reserve_edges(std::size_t n) {
        m_edges.reserve(n);
    }

    template<node TNode, number TWeight>
    template<typename ... TExtension>
    constexpr wdigraph<TNode, TWeight, TExtension...> wdigraph_builder<TNode, TWeight>::compile() {
        using size_type = wdigraph<TNode, TWeight, TExtension...>::size_type;
        wdigraph<TNode, TWeight, TExtension...> result;

        const auto V = m_nodes.size();
        const auto E = m_edges.size();

        result.m_nodes = std::move(m_nodes);

        result.m_offsets.assign(V + 1, 0);
        result.m_edges.resize(E);
        result.m_weights.resize(E);

        for (const auto& e : m_edges) {
            ++result.m_offsets[e.from + 1];
        }
        for (size_type i = 1; i <= V; ++i) {
            result.m_offsets[i] += result.m_offsets[i - 1];
        }

        auto fwd_offsets = result.m_offsets;
        for (const auto& e : m_edges) {
            auto idx = fwd_offsets[e.from]++;
            result.m_edges[idx] = e.to;
            result.m_weights[idx] = e.weight;
        }

        result.m_rev_offsets.assign(V + 1, 0);
        result.m_rev_edges.resize(E);
        result.m_rev_weights.resize(E);

        for (const auto& e : m_edges) {
            ++result.m_rev_offsets[e.to + 1];
        }
        for (size_type i = 1; i <= V; ++i) {
            result.m_rev_offsets[i] += result.m_rev_offsets[i - 1];
        }

        auto rev_offsets = result.m_rev_offsets;
        for (const auto& e : m_edges) {
            auto idx = rev_offsets[e.to]++;
            result.m_rev_edges[idx] = e.from;
            result.m_rev_weights[idx] = e.weight;
        }

        m_edges.clear();
        return result;
    }

    template<node TNode, number TWeight, typename ... TExtension>
    constexpr const wdigraph<TNode, TWeight, TExtension...>::node_type &wdigraph<TNode, TWeight, TExtension...>::node(nodeid_t id) const {
        if (m_nodes.size() <= id) {
            return std::unexpected{"Index is out of range."};
        }

        return m_nodes[id];
    }

    template<node TNode, number TWeight, typename ... TExtension>
    constexpr wdigraph<TNode, TWeight, TExtension...>::size_type wdigraph<TNode, TWeight, TExtension...>::count() const {
        return m_nodes.size();
    }

    template<node TNode, number TWeight, typename ... TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string> wdigraph<TNode, TWeight, TExtension...>::out_edges(
        const nodeid_t id) const {
        if (m_offsets.size() - 1 <= id) {
            return std::unexpected{"Index is out of range."};
        }
        const auto start = m_offsets[id];
        const auto end = m_offsets[id + 1];
        return std::span {m_edges.begin() + start, end - start};
    }

    template<node TNode, number TWeight, typename ... TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string> wdigraph<TNode, TWeight, TExtension...>::in_edges(
        const nodeid_t id) const {
        if (m_rev_offsets.size() - 1 <= id) {
            return std::unexpected{"Index is out of range."};
        }
        const auto start = m_rev_offsets[id];
        const auto end = m_rev_offsets[id + 1];
        return std::span {m_rev_edges.begin() + start, end - start};
    }

    template<node TNode, number TWeight, typename ... TExtension>
    constexpr std::expected<std::span<const typename wdigraph<TNode, TWeight, TExtension...>::weight_type>, std::string>
    wdigraph<TNode, TWeight, TExtension...>::out_weights(nodeid_t id) const {
        if (m_weights.size() - 1 <= id) {
            return std::unexpected{"Index is out of range."};
        }
        const auto start = m_weights[id];
        const auto end = m_weights[id + 1];
        return std::span {m_weights.begin() + start, end - start};
    }

    template<node TNode, number TWeight, typename ... TExtension>
    constexpr std::expected<std::span<const typename wdigraph<TNode, TWeight, TExtension...>::weight_type>, std::string>
    wdigraph<TNode, TWeight, TExtension...>::in_weights(nodeid_t id) const {
        if (m_rev_weights.size() - 1 <= id) {
            return std::unexpected{"Index is out of range."};
        }
        const auto start = m_rev_weights[id];
        const auto end = m_rev_weights[id + 1];
        return std::span {m_rev_edges.begin() + start, end - start};
    }

    template<node TNode, number TWeight, typename ... TExtension>
    constexpr bool wdigraph<TNode, TWeight, TExtension...>::is_acyclic() const {
        const auto n = count();
        if (n == 0) return true;

        std::vector<size_type> in_degree(n);
        std::vector<nodeid_t> queue;
        queue.reserve(n);

        for (nodeid_t u = 0; u < n; ++u) {
            in_degree[u] = m_rev_offsets[u + 1] - m_rev_offsets[u];
            if (in_degree[u] == 0) {
                queue.emplace_back(u);
            }
        }
        size_type head = 0;
        while (head < queue.size()) {
            const auto u = queue[head++];

            if (auto edges = out_edges(u)) {
                for (const auto v : *edges) {
                    if (--in_degree[v] == 0) {
                        queue.emplace_back(v);
                    }
                }
            }
        }
        return queue.size() == n;
    }

    struct _static_test_node {
        int id;
    };
    static_assert(directed_graph<digraph<_static_test_node>>,
        "Immutable CSR graph does not satisfy `directed_graph` concept");
    static_assert(wdirected_graph<wdigraph<_static_test_node, int>>,
        "Immutable CSR graph does not satisfy `wdirected_graph` concept");
}

#endif //DAGPP_CSR_H