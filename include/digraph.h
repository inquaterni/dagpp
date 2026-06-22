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
        constexpr size_type node_count() const { return m_nodes.size() - m_free_nodes.size(); }
        [[nodiscard]]
        constexpr size_type edge_count() const {
            size_type count = 0;
            for (const auto& edges : m_out_edges) count += edges.size();
            return count;
        }

        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> out_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> in_edges(nodeid_t id) const;

        constexpr void remove_edge(nodeid_t from, nodeid_t to);
        constexpr void remove_node(nodeid_t id);

        [[nodiscard]]
        constexpr bool is_acyclic() const;

    private:
        std::vector<node_type>                 m_nodes;
        std::vector<bool>                      m_valid_nodes;
        std::vector<nodeid_t>                  m_free_nodes;
        std::vector<std::vector<nodeid_t>>     m_out_edges;
        std::vector<std::vector<nodeid_t>>     m_in_edges;
    };

    template <typename TNode, number TWeight, typename ...TExtension>
    class wdigraph: public TExtension... {
        public:
        using node_type   = TNode;
        using size_type   = std::size_t;
        using weight_type = TWeight;

        constexpr nodeid_t add_node(const TNode &node);
        constexpr nodeid_t add_node(TNode &&node);
        constexpr void add_edge(nodeid_t from, nodeid_t to, TWeight weight);
        constexpr void reserve_nodes(size_type n);

        [[nodiscard]]
        constexpr const node_type& node(nodeid_t id) const { return m_nodes[id]; }
        [[nodiscard]]
        constexpr node_type& node(nodeid_t id) { return m_nodes[id]; }
        [[nodiscard]]
        constexpr size_type node_count() const { return m_nodes.size() - m_free_nodes.size(); }
        [[nodiscard]]
        constexpr size_type edge_count() const {
            size_type count = 0;
            for (const auto& edges : m_out_edges) count += edges.size();
            return count;
        }

        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> out_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const nodeid_t>, std::string> in_edges(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const TWeight>, std::string> out_weights(nodeid_t id) const;
        [[nodiscard]]
        constexpr std::expected<std::span<const TWeight>, std::string> in_weights(nodeid_t id) const;

        constexpr void remove_edge(nodeid_t from, nodeid_t to);
        constexpr void remove_node(nodeid_t id);

        [[nodiscard]]
        constexpr bool is_acyclic() const;

        private:
        std::vector<node_type>                 m_nodes;
        std::vector<bool>                      m_valid_nodes;
        std::vector<nodeid_t>                  m_free_nodes;
        std::vector<std::vector<nodeid_t>>     m_out_edges;
        std::vector<std::vector<TWeight>>      m_out_weights;
        std::vector<std::vector<nodeid_t>>     m_in_edges;
        std::vector<std::vector<TWeight>>      m_in_weights;
    };

    template<typename TNode, typename ... TExtension>
    constexpr nodeid_t digraph<TNode, TExtension...>::add_node(const TNode &node) {
        if (!m_free_nodes.empty()) {
            const auto id = m_free_nodes.back();
            m_free_nodes.pop_back();
            m_nodes[id] = node;
            m_valid_nodes[id] = true;
            return id;
        }
        m_nodes.emplace_back(node);
        m_valid_nodes.push_back(true);
        m_out_edges.emplace_back();
        m_in_edges.emplace_back();
        return m_nodes.size() - 1;
    }

    template<typename TNode, typename ... TExtension>
    constexpr nodeid_t digraph<TNode, TExtension...>::add_node(TNode &&node) {
        if (!m_free_nodes.empty()) {
            const auto id = m_free_nodes.back();
            m_free_nodes.pop_back();
            m_nodes[id] = std::move(node);
            m_valid_nodes[id] = true;
            return id;
        }
        m_nodes.emplace_back(std::move(node));
        m_valid_nodes.push_back(true);
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
        m_valid_nodes.reserve(n);
        m_out_edges.reserve(n);
        m_in_edges.reserve(n);
    }

    template<typename TNode, typename ... TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string> digraph<TNode, TExtension...>::out_edges(
    const nodeid_t id) const {
        if (id >= m_out_edges.size() || !m_valid_nodes[id]) return std::unexpected("Index is out of range or node is invalid.");
        return m_out_edges[id];
    }

    template<typename TNode, typename ... TExtension>
    constexpr void digraph<TNode, TExtension...>::remove_edge(const nodeid_t from, const nodeid_t to) {
        if (from >= m_out_edges.size() || to >= m_in_edges.size()) return;
        auto& out = m_out_edges[from];
        for (size_type i = out.size(); i-- > 0;) {
            if (out[i] == to) out.erase(out.begin() + i);
        }
        auto& in = m_in_edges[to];
        for (size_type i = in.size(); i-- > 0;) {
            if (in[i] == from) in.erase(in.begin() + i);
        }
    }

    template<typename TNode, typename ... TExtension>
    constexpr void digraph<TNode, TExtension...>::remove_node(const nodeid_t id) {
        if (id >= m_out_edges.size() || !m_valid_nodes[id]) return;
        m_valid_nodes[id] = false;
        m_free_nodes.push_back(id);
        
        for (nodeid_t to : m_out_edges[id]) {
            auto& in = m_in_edges[to];
            for (size_type i = in.size(); i-- > 0;) {
                if (in[i] == id) in.erase(in.begin() + i);
            }
        }
        m_out_edges[id].clear();

        for (nodeid_t from : m_in_edges[id]) {
            auto& out = m_out_edges[from];
            for (size_type i = out.size(); i-- > 0;) {
                if (out[i] == id) out.erase(out.begin() + i);
            }
        }
        m_in_edges[id].clear();
    }

    template<typename TNode, typename ... TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string> digraph<TNode, TExtension...>::in_edges(
    const nodeid_t id) const {
        if (id >= m_in_edges.size() || !m_valid_nodes[id]) return std::unexpected("Index is out of range or node is invalid.");
        return m_in_edges[id];
    }

    template<typename TNode, typename ... TExtension>
    constexpr bool digraph<TNode, TExtension...>::is_acyclic() const {
        const auto max_id = m_nodes.size();
        const auto n = node_count();
        if (n == 0) return true;

        std::vector<size_type> in_degree(max_id);
        std::vector<nodeid_t> queue;
        queue.reserve(n);

        for (nodeid_t u = 0; u < max_id; ++u) {
            if (!m_valid_nodes[u]) continue;
            in_degree[u] = m_in_edges[u].size();
            if (in_degree[u] == 0) queue.emplace_back(u);
        }

        size_type head = 0;
        size_type processed_count = 0;
        while (head < queue.size()) {
            const auto u = queue[head++];
            processed_count++;
            for (const auto v : m_out_edges[u]) {
                if (--in_degree[v] == 0) queue.emplace_back(v);
            }
        }
        return processed_count == n;
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr nodeid_t wdigraph<TNode, TWeight, TExtension...>::add_node(const TNode &node) {
        if (!m_free_nodes.empty()) {
            const auto id = m_free_nodes.back();
            m_free_nodes.pop_back();
            m_nodes[id] = node;
            m_valid_nodes[id] = true;
            return id;
        }
        m_nodes.emplace_back(node);
        m_valid_nodes.push_back(true);
        m_out_edges.emplace_back();
        m_out_weights.emplace_back();
        m_in_edges.emplace_back();
        m_in_weights.emplace_back();
        return m_nodes.size() - 1;
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr nodeid_t wdigraph<TNode, TWeight, TExtension...>::add_node(TNode &&node) {
        if (!m_free_nodes.empty()) {
            const auto id = m_free_nodes.back();
            m_free_nodes.pop_back();
            m_nodes[id] = std::move(node);
            m_valid_nodes[id] = true;
            return id;
        }
        m_nodes.emplace_back(std::move(node));
        m_valid_nodes.push_back(true);
        m_out_edges.emplace_back();
        m_out_weights.emplace_back();
        m_in_edges.emplace_back();
        m_in_weights.emplace_back();
        return m_nodes.size() - 1;
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr void wdigraph<TNode, TWeight, TExtension...>::add_edge(
        const nodeid_t from, const nodeid_t to, const TWeight weight) {
        m_out_edges[from].emplace_back(to);
        m_out_weights[from].emplace_back(weight);
        m_in_edges[to].emplace_back(from);
        m_in_weights[to].emplace_back(weight);
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr void wdigraph<TNode, TWeight, TExtension...>::reserve_nodes(size_type n) {
        m_nodes.reserve(n);
        m_valid_nodes.reserve(n);
        m_out_edges.reserve(n);
        m_out_weights.reserve(n);
        m_in_edges.reserve(n);
        m_in_weights.reserve(n);
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string>
    wdigraph<TNode, TWeight, TExtension...>::out_edges(const nodeid_t id) const {
        if (id >= m_out_edges.size() || !m_valid_nodes[id]) return std::unexpected("Index is out of range or node is invalid.");
        return m_out_edges[id];
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr std::expected<std::span<const nodeid_t>, std::string>
    wdigraph<TNode, TWeight, TExtension...>::in_edges(const nodeid_t id) const {
        if (id >= m_in_edges.size() || !m_valid_nodes[id]) return std::unexpected("Index is out of range or node is invalid.");
        return m_in_edges[id];
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr std::expected<std::span<const TWeight>, std::string>
    wdigraph<TNode, TWeight, TExtension...>::out_weights(const nodeid_t id) const {
        if (id >= m_out_weights.size() || !m_valid_nodes[id]) return std::unexpected("Index is out of range or node is invalid.");
        return m_out_weights[id];
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr std::expected<std::span<const TWeight>, std::string>
    wdigraph<TNode, TWeight, TExtension...>::in_weights(const nodeid_t id) const {
        if (id >= m_in_weights.size() || !m_valid_nodes[id]) return std::unexpected("Index is out of range or node is invalid.");
        return m_in_weights[id];
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr void wdigraph<TNode, TWeight, TExtension...>::remove_edge(const nodeid_t from, const nodeid_t to) {
        if (from >= m_out_edges.size() || to >= m_in_edges.size()) return;
        
        for (size_type i = m_out_edges[from].size(); i-- > 0;) {
            if (m_out_edges[from][i] == to) {
                m_out_edges[from].erase(m_out_edges[from].begin() + i);
                m_out_weights[from].erase(m_out_weights[from].begin() + i);
            }
        }
        for (size_type i = m_in_edges[to].size(); i-- > 0;) {
            if (m_in_edges[to][i] == from) {
                m_in_edges[to].erase(m_in_edges[to].begin() + i);
                m_in_weights[to].erase(m_in_weights[to].begin() + i);
            }
        }
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr void wdigraph<TNode, TWeight, TExtension...>::remove_node(const nodeid_t id) {
        if (id >= m_out_edges.size() || !m_valid_nodes[id]) return;
        m_valid_nodes[id] = false;
        m_free_nodes.push_back(id);
        
        for (nodeid_t to : m_out_edges[id]) {
            for (size_type i = m_in_edges[to].size(); i-- > 0;) {
                if (m_in_edges[to][i] == id) {
                    m_in_edges[to].erase(m_in_edges[to].begin() + i);
                    m_in_weights[to].erase(m_in_weights[to].begin() + i);
                }
            }
        }
        m_out_edges[id].clear();
        m_out_weights[id].clear();

        for (nodeid_t from : m_in_edges[id]) {
            for (size_type i = m_out_edges[from].size(); i-- > 0;) {
                if (m_out_edges[from][i] == id) {
                    m_out_edges[from].erase(m_out_edges[from].begin() + i);
                    m_out_weights[from].erase(m_out_weights[from].begin() + i);
                }
            }
        }
        m_in_edges[id].clear();
        m_in_weights[id].clear();
    }

    template<typename TNode, number TWeight, typename ...TExtension>
    constexpr bool wdigraph<TNode, TWeight, TExtension...>::is_acyclic() const {
        const auto max_id = m_nodes.size();
        const auto n = node_count();
        if (n == 0) return true;

        std::vector<size_type> in_degree(max_id);
        std::vector<nodeid_t>  queue;
        queue.reserve(n);

        for (nodeid_t u = 0; u < max_id; ++u) {
            if (!m_valid_nodes[u]) continue;
            in_degree[u] = m_in_edges[u].size();
            if (in_degree[u] == 0) queue.emplace_back(u);
        }

        size_type head = 0;
        size_type processed_count = 0;
        while (head < queue.size()) {
            const auto u = queue[head++];
            processed_count++;
            for (const auto v : m_out_edges[u]) {
                if (--in_degree[v] == 0) queue.emplace_back(v);
            }
        }
        return processed_count == n;
    }


    struct _static_test_node {
        int id;
    };
    static_assert(directed_graph<digraph<_static_test_node>>,
        "Mutable digraph does not satisfy `directed_graph` concept");
    static_assert(wdirected_graph<wdigraph<_static_test_node, int>>,
        "Mutable wdigraph does not satisfy `wdirected_graph` concept");

} // dagpp

#endif //DAGPP_DIRECTED_GRAPH_H