//
// Created by inquaterni on 3/17/26.
//

#ifndef DAGPP_EXT_H
#define DAGPP_EXT_H
#include <filesystem>
#include <fstream>
#include <format>

#include "digraph.h"

namespace dagpp::ext {
    template<typename TGraph, typename Pred>
    concept dot_label_predicate =
        directed_graph<TGraph> &&
        std::invocable<Pred, typename TGraph::size_type, typename TGraph::node_type> &&
        printable<std::invoke_result_t<Pred, typename TGraph::size_type, typename TGraph::node_type>>;

    class dot_exporter {
    public:
        template<typename TDir = outbound, directed_graph TSelf, typename Pred>
        requires dot_label_predicate<TSelf, Pred>
        constexpr void to_dot(this const TSelf &self, const Pred& label_pred, std::ofstream &out, TDir cmp = TDir {}) {
            out << "digraph G {\n";
            for (typename TSelf::size_type i = 0; i < self.count(); ++i) {
                out << label_pred(i, self.node(i));
                const auto edges = cmp(self, i);
                if (!edges) continue;

                for (const auto &target : *edges) {
                    out << std::format("\tn{} -> n{};\n", i, target);
                }
            }
            out << "}\n";
        }
    };
} // dagpp::ext

#endif //DAGPP_EXT_H