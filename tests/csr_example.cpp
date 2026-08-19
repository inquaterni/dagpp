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

#include <iostream>
#include <dagpp.h>

#include "gtest/gtest.h"

// Node data must satisfy std::semiregular ONLY for the CSR graph
struct Node {
    int id;
    std::string name;
};

TEST(csr_example_test, main) {
    dagpp::csr::digraph_builder<Node> builder;

    // Node accumulation is mutable...
    auto a = builder.add_node({0});
    auto b = builder.add_node({1});
    auto c = builder.add_node({2});

    builder.add_edge(a, b);
    builder.add_edge(b, c);

    // ...until compilation into an immutable graph
    const auto graph = builder.compile();
    // When using CSR graphs, in order to mix in extensions, you need to pass them to the builder's `compile()` method
    // const auto graph_with_ext = builder.compile<dagpp::ext::dot_exporter>();

    std::cout << "CSR graph has " << graph.node_count() << " nodes.\n";

    ASSERT_EQ(graph.node_count(), 3);
}