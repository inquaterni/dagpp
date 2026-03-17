//
// Created by inquaterni on 3/17/26.
//
#include <iostream>
#include "../include/digraph.h"
#include "../include/topo_sort.h"
#include "../include/dot.h" // Optional: for DOT export
#include "gtest/gtest.h"


// Node data
// NOTE: as of now node type MUST be trivial.
struct my_node {
    int id;
};

TEST(basic_usage, full_program_test) {
    dagpp::digraph_builder<my_node> builder;

    auto a = builder.add_node({10});
    auto b = builder.add_node({20});
    auto c = builder.add_node({30});

    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, c);

    // Compile graph with DOT export extension
    const auto graph = builder.compile<dagpp::ext::dot_exporter>();

    if (auto out = graph.out_edges(a); out.has_value()) {
        std::cout << "Node A has " << out->size() << " outgoing edges.\n";
    }

    if (auto order = dagpp::topo_sort(graph); order.has_value()) {
        std::cout << "Graph is a DAG!\n";
    }

    std::ofstream out("graph.dot");
    ASSERT_TRUE(out.is_open());
    graph.to_dot([](const std::size_t index, const my_node& node) {
        return std::format("\tn{} [label=\"{}\"];\n", index, node.id);
    }, out);
    out.close();

    ASSERT_TRUE(std::filesystem::exists("graph.dot"));
    std::filesystem::remove("graph.dot");
}