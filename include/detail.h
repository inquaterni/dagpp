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

#ifndef DAGPP_DETAIL_H
#define DAGPP_DETAIL_H
#include "helpers.h"
#include <memory_resource>

namespace dagpp::detail {
    template<typename T>
    concept node = std::semiregular<T>;

    template <node TNode>
    class base_builder {
    public:
        using allocator_type = std::pmr::polymorphic_allocator<TNode>;

        constexpr explicit base_builder(allocator_type alloc = {})
            : m_nodes(alloc) {}

        constexpr nodeid_t add_node(const TNode &node);
        constexpr nodeid_t add_node(TNode &&node);

        template<class ... Args>
        constexpr nodeid_t emplace_node(Args && ...args);

        constexpr void reserve_nodes(std::size_t n);
    protected:
        std::pmr::vector<TNode> m_nodes;
    };

    template<node TNode>
    constexpr nodeid_t base_builder<TNode>::add_node(const TNode &node) {
        m_nodes.push_back(node);
        return m_nodes.size() - 1;
    }
    template<node TNode>
    constexpr nodeid_t base_builder<TNode>::add_node(TNode &&node) {
        m_nodes.emplace_back(std::forward<TNode>(node));
        return m_nodes.size() - 1;
    }
    template<node TNode>
    template<typename... Args>
    constexpr nodeid_t base_builder<TNode>::emplace_node(Args&& ...args) {
        m_nodes.emplace_back(std::forward<Args>(args)...);
        return m_nodes.size() - 1;
    }
    template<node TNode>
    constexpr void base_builder<TNode>::reserve_nodes(std::size_t n) {
        m_nodes.reserve(n);
    }
}

#endif // DAGPP_DETAIL_H
