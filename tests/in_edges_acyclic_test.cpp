//
// Created by inquaterni on 3/17/26.
//
#include <gtest/gtest.h>
#include "../include/digraph.h"
#include "global.h"

TEST(digraph_test, in_edges_diamond) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, d);
    builder.add_edge(c, d);
    const auto graph = builder.compile();

    // A has no predecessors
    const auto a_in = graph.in_edges(a);
    ASSERT_TRUE(a_in.has_value());
    EXPECT_TRUE(a_in->empty());

    // B's only predecessor is A
    const auto b_in = graph.in_edges(b);
    ASSERT_TRUE(b_in.has_value());
    ASSERT_EQ(b_in->size(), 1);
    EXPECT_EQ((*b_in)[0], a);

    // C's only predecessor is A
    const auto c_in = graph.in_edges(c);
    ASSERT_TRUE(c_in.has_value());
    ASSERT_EQ(c_in->size(), 1);
    EXPECT_EQ((*c_in)[0], a);

    // D has two predecessors: B and C
    const auto d_in = graph.in_edges(d);
    ASSERT_TRUE(d_in.has_value());
    ASSERT_EQ(d_in->size(), 2);
}

TEST(digraph_test, in_edges_out_of_range) {
    dagpp::digraph_builder<test_node> builder;
    builder.add_node({0});
    const auto graph = builder.compile();

    const auto result = graph.in_edges(99);
    ASSERT_FALSE(result.has_value());
}

TEST(digraph_test, is_acyclic_dag) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, d);
    builder.add_edge(c, d);
    const auto graph = builder.compile();

    EXPECT_TRUE(graph.is_acyclic());
}

TEST(digraph_test, is_acyclic_cycle) {
    // 0 -> 1 -> 2 -> 0
    dagpp::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    builder.add_edge(a, b);
    builder.add_edge(b, c);
    builder.add_edge(c, a);
    const auto graph = builder.compile();

    EXPECT_FALSE(graph.is_acyclic());
}

TEST(digraph_test, is_acyclic_empty) {
    dagpp::digraph_builder<test_node> builder;
    const auto graph = builder.compile();

    EXPECT_TRUE(graph.is_acyclic());
}

TEST(digraph_test, is_acyclic_self_loop) {
    dagpp::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    builder.add_edge(a, a);
    const auto graph = builder.compile();

    EXPECT_FALSE(graph.is_acyclic());
}
