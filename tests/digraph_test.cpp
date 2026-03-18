#include <gtest/gtest.h>
#include "../include/digraph.h"
#include "global.h"

TEST(digraph_test, empty_graph) {
    const dagpp::digraph<test_node> graph;

    EXPECT_EQ(graph.count(), 0);
}

TEST(digraph_test, single_node) {
    dagpp::digraph<test_node> graph;
    const auto id = graph.add_node({42});

    EXPECT_EQ(graph.count(), 1);
    EXPECT_EQ(graph.node(id).value, 42);
}

TEST(digraph_test, multiple_nodes) {
    dagpp::digraph<test_node> graph;
    const auto a = graph.add_node({1});
    const auto b = graph.add_node({2});
    const auto c = graph.add_node({3});

    EXPECT_EQ(graph.count(), 3);
    EXPECT_EQ(graph.node(a).value, 1);
    EXPECT_EQ(graph.node(b).value, 2);
    EXPECT_EQ(graph.node(c).value, 3);
}

TEST(digraph_test, single_edge) {
    dagpp::digraph<test_node> graph;
    const auto a = graph.add_node({1});
    const auto b = graph.add_node({2});
    graph.add_edge(a, b);

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 1);
    EXPECT_EQ((*a_out)[0], b);

    const auto b_out = graph.out_edges(b);
    ASSERT_TRUE(b_out.has_value());
    EXPECT_TRUE(b_out->empty());
}

TEST(digraph_test, multiple_out_edges) {
    dagpp::digraph<test_node> graph;
    const auto a = graph.add_node({});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    const auto d = graph.add_node({3});
    graph.add_edge(a, b);
    graph.add_edge(a, c);
    graph.add_edge(a, d);

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 3);
    EXPECT_EQ((*a_out)[0], b);
    EXPECT_EQ((*a_out)[1], c);
    EXPECT_EQ((*a_out)[2], d);
}

TEST(digraph_test, diamond_graph) {
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

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 2);
    EXPECT_EQ((*a_out)[0], b);
    EXPECT_EQ((*a_out)[1], c);

    const auto b_out = graph.out_edges(b);
    ASSERT_TRUE(b_out.has_value());
    ASSERT_EQ(b_out->size(), 1);
    EXPECT_EQ((*b_out)[0], d);

    const auto c_out = graph.out_edges(c);
    ASSERT_TRUE(c_out.has_value());
    ASSERT_EQ(c_out->size(), 1);
    EXPECT_EQ((*c_out)[0], d);

    const auto d_out = graph.out_edges(d);
    EXPECT_TRUE(d_out->empty());
}
