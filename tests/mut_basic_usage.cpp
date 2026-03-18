#include <iostream>
#include "../include/digraph.h"
#include "../include/topo_sort.h"
#include "../include/dot.h" // Optional: for DOT export
#include "gtest/gtest.h"


// Node data
// NOTE: node type doesn't have to be trivial, but should be semiregular
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
