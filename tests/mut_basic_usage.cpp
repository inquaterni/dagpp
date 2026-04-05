#include <vector>
#include <string>
#include <expected>
#include <span>
#include <limits>
#include <filesystem>
#include <fstream>
#include <format>
#include <cstddef>

#include <iostream>
#include "gtest/gtest.h"

import dagpp.digraph;
import dagpp.topo_sort;
import dagpp.ext.dot;
import dagpp.usings;

struct my_node {
    int id;
};

TEST(basic_usage_mut_test, full_program_test) {
    dagpp::digraph<my_node, dagpp::ext::dot_exporter> graph;

    auto a = graph.add_node({10});
    auto b = graph.add_node({20});
    auto c = graph.add_node({30});

    graph.add_edge(a, b);
    graph.add_edge(a, c);
    graph.add_edge(b, c);

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
