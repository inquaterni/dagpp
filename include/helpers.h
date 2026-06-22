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

#ifndef DAGPP_HELPERS_H
#define DAGPP_HELPERS_H
#include <span>
#include "usings.h"

// ============================================================================
// Microsoft Guidelines Support Library (GSL) - narrow_cast
// Copyright (c) 2015 Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
// Source: https://github.com/microsoft/GSL
// ============================================================================
#if defined(__clang__)
    #define GLS_SUPPRESS(x) [[gsl::suppress(#x)]]
#elif defined(_MSC_VER) && _MSC_VER >= 1950
    // Visual Studio versions after 2022 (_MSC_VER > 1944) support the justification message.
    #define GLS_SUPPRESS(x) [[gsl::suppress(#x)]]
#elif defined(_MSC_VER) && !defined(__INTEL_COMPILER) && !defined(__NVCC__)
    #define GLS_SUPPRESS(x) [[gsl::suppress(x)]]
#else
    #define GLS_SUPPRESS(x)
#endif // defined(__clang__)

namespace gsl {
    template <class T, class U>
    GLS_SUPPRESS(type.1) constexpr T narrow_cast(U&& u) noexcept
    {
        return static_cast<T>(std::forward<U>(u));
    }
} // gsl

namespace dagpp {
    template<typename T>
    concept number = std::integral<T> || std::floating_point<T>;

    template <typename T>
    concept directed_graph = requires (const T &t, nodeid_t id)
    {
        typename T::node_type;
        typename T::size_type;
        {*t.out_edges(id)} -> std::convertible_to<std::span<const nodeid_t>>;
        {*t.in_edges(id)} -> std::convertible_to<std::span<const nodeid_t>>;
        {t.is_acyclic()} -> std::same_as<bool>;
        {t.node(id)} -> std::convertible_to<typename T::node_type>;
        {t.node_count()} -> std::same_as<typename T::size_type>;
        {t.edge_count()} -> std::same_as<typename T::size_type>;
    };

    template<typename T>
    concept wdirected_graph = directed_graph<T> && requires (const T &t, nodeid_t id)
    {
        typename T::weight_type;
        requires number<typename T::weight_type>;
        {*t.out_weights(id)} -> std::convertible_to<std::span<const typename T::weight_type>>;
        {*t.in_weights(id)} -> std::convertible_to<std::span<const typename T::weight_type>>;
    };

    template<typename T>
    concept printable = requires (std::ostream &os, const T &val)
    {
        {os << val} -> std::convertible_to<std::ostream &>;
    };

    struct outbound {
        template<directed_graph TGraph>
        constexpr auto operator()(const TGraph &g, nodeid_t node) const {
            return g.out_edges(node);
        }
    };

    struct inbound {
        template<directed_graph TGraph>
        constexpr auto operator()(const TGraph &g, nodeid_t node) const {
            return g.in_edges(node);
        }
    };
} // dagpp

#endif //DAGPP_HELPERS_H