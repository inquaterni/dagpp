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