//
// Created by inquaterni on 3/17/26.
//
#include <gtest/gtest.h>
#include "../include/csr.h"
#include "../include/digraph.h"
#include "global.h"

TEST(csr_digraph_test, in_edges_diamond) {
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
TEST(csr_digraph_test, in_edges_out_of_range) {
    dagpp::csr::digraph_builder<test_node> builder;
    builder.add_node({0});
    const auto graph = builder.compile();

    const auto result = graph.in_edges(99);
    ASSERT_FALSE(result.has_value());
}
TEST(csr_digraph_test, is_acyclic_dag) {
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

    EXPECT_TRUE(graph.is_acyclic());
}
TEST(csr_digraph_test, is_acyclic_cycle) {
    // 0 -> 1 -> 2 -> 0
    dagpp::csr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    builder.add_edge(a, b);
    builder.add_edge(b, c);
    builder.add_edge(c, a);
    const auto graph = builder.compile();

    EXPECT_FALSE(graph.is_acyclic());
}
TEST(csr_digraph_test, is_acyclic_empty) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto graph = builder.compile();

    EXPECT_TRUE(graph.is_acyclic());
}
TEST(csr_digraph_test, is_acyclic_self_loop) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    builder.add_edge(a, a);
    const auto graph = builder.compile();

    EXPECT_FALSE(graph.is_acyclic());
}

TEST(mut_digraph_test, in_edges_diamond) {
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
TEST(mut_digraph_test, in_edges_out_of_range) {
    dagpp::digraph<test_node> graph;
    graph.add_node({0});

    const auto result = graph.in_edges(99);
    ASSERT_FALSE(result.has_value());
}
TEST(mut_digraph_test, is_acyclic_dag) {
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

    EXPECT_TRUE(graph.is_acyclic());
}
TEST(mut_digraph_test, is_acyclic_cycle) {
    // 0 -> 1 -> 2 -> 0
    dagpp::digraph<test_node> graph;
    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    graph.add_edge(a, b);
    graph.add_edge(b, c);
    graph.add_edge(c, a);

    EXPECT_FALSE(graph.is_acyclic());
}
TEST(mut_digraph_test, is_acyclic_empty) {
    dagpp::digraph<test_node> graph;

    EXPECT_TRUE(graph.is_acyclic());
}
TEST(mut_digraph_test, is_acyclic_self_loop) {
    dagpp::digraph<test_node> graph;
    const auto a = graph.add_node({0});
    graph.add_edge(a, a);

    EXPECT_FALSE(graph.is_acyclic());
}

