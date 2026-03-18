//
// Created by inquaterni on 3/17/26.
//
#include <gtest/gtest.h>
#include "../include/csr.h"
#include "../include/digraph.h"
#include "../include/topo_sort.h"
#include "global.h"

TEST(topo_sort_test, empty_csr_graph) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto graph = builder.compile();
    const auto result = dagpp::topo_sort(graph);

    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->empty());
}
TEST(topo_sort_test, csr_graph_single_node) {
    dagpp::csr::digraph_builder<test_node> builder;
    builder.add_node({1});
    const auto graph = builder.compile();
    const auto result = dagpp::topo_sort(graph);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 1);
    EXPECT_EQ((*result)[0], 0);
}
TEST(topo_sort_test, csr_linear_chain) {
    // 0 -> 1 -> 2 -> 3
    dagpp::csr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b);
    builder.add_edge(b, c);
    builder.add_edge(c, d);
    const auto graph = builder.compile();
    const auto result = dagpp::topo_sort(graph);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 4);
    EXPECT_EQ((*result)[0], a);
    EXPECT_EQ((*result)[1], b);
    EXPECT_EQ((*result)[2], c);
    EXPECT_EQ((*result)[3], d);
}
TEST(topo_sort_test, csr_diamond_dag) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::csr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, d);
    builder.add_edge(c, d);
    const auto graph = builder.compile();
    const auto result = dagpp::topo_sort(graph);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 4);
    // A must come first, D must come last
    EXPECT_EQ((*result)[0], a);
    EXPECT_EQ((*result)[3], d);
}
TEST(topo_sort_test, csr_cycle_detected) {
    // 0 -> 1 -> 2 -> 0
    dagpp::csr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    builder.add_edge(a, b);
    builder.add_edge(b, c);
    builder.add_edge(c, a);
    const auto graph = builder.compile();
    const auto result = dagpp::topo_sort(graph);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "Graph contains a cycle.");
}

TEST(topo_sort_test, empty_graph) {
    dagpp::digraph<test_node> graph;
    const auto result = dagpp::topo_sort(graph);

    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->empty());
}
TEST(topo_sort_test, single_node) {
    dagpp::digraph<test_node> graph;
    graph.add_node({1});
    const auto result = dagpp::topo_sort(graph);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 1);
    EXPECT_EQ((*result)[0], 0);
}
TEST(topo_sort_test, linear_chain) {
    // 0 -> 1 -> 2 -> 3
    dagpp::digraph<test_node> graph;
    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    const auto d = graph.add_node({3});
    graph.add_edge(a, b);
    graph.add_edge(b, c);
    graph.add_edge(c, d);
    const auto result = dagpp::topo_sort(graph);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 4);
    EXPECT_EQ((*result)[0], a);
    EXPECT_EQ((*result)[1], b);
    EXPECT_EQ((*result)[2], c);
    EXPECT_EQ((*result)[3], d);
}
TEST(topo_sort_test, diamond_dag) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::digraph<test_node> graph;
    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    const auto d = graph.add_node({3});
    graph.add_edge(a, b);
    graph.add_edge(a, c);
    graph.add_edge(b, d);
    graph.add_edge(c, d);
    const auto result = dagpp::topo_sort(graph);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 4);
    // A must come first, D must come last
    EXPECT_EQ((*result)[0], a);
    EXPECT_EQ((*result)[3], d);
}
TEST(topo_sort_test, cycle_detected) {
    // 0 -> 1 -> 2 -> 0
    dagpp::digraph<test_node> graph;
    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    graph.add_edge(a, b);
    graph.add_edge(b, c);
    graph.add_edge(c, a);
    const auto result = dagpp::topo_sort(graph);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "Graph contains a cycle.");
}

struct MockErrorGraph {
    using node_type = test_node;
    using size_type = std::size_t;
    std::size_t count() const { return 1; }
    test_node node(dagpp::nodeid_t) const { return {0}; }
    std::expected<std::span<const dagpp::nodeid_t>, std::string> out_edges(dagpp::nodeid_t) const {
        return std::unexpected{"Mock error"};
    }
    std::expected<std::span<const dagpp::nodeid_t>, std::string> in_edges(dagpp::nodeid_t) const {
        return std::unexpected{"Mock error"};
    }
    bool is_acyclic() const { return true; }
};

TEST(topo_sort_test, error_graph) {
    MockErrorGraph g;
    const auto result = dagpp::topo_sort(g);
    
    // out_edges returns error, which topo_sort skips using `continue`.
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 1);
    EXPECT_EQ((*result)[0], 0);
}