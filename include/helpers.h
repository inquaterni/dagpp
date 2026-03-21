//
// Created by inquaterni on 3/18/26.
//

#ifndef DAGPP_HELPERS_H
#define DAGPP_HELPERS_H
#include <span>
#include "usings.h"

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
        {t.count()} -> std::same_as<typename T::size_type>;
    };

    template<typename T>
    concept wdirected_graph = directed_graph<T> && requires (const T &t, nodeid_t id)
    {
        typename T::weight_type;
        requires number<typename T::weight_type>;
        {*t.out_weights(id)} -> std::convertible_to<std::span<const typename T::weight_type>>;
        {*t.in_weights(id)} -> std::convertible_to<std::span<const typename T::weight_type>>;
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