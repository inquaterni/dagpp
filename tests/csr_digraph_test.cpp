#include <gtest/gtest.h>
#include "../include/csr.h"
#include "global.h"

TEST(csr_digraph_test, empty_graph) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto graph = builder.compile();

    EXPECT_EQ(graph.count(), 0);
}

TEST(csr_digraph_test, single_node) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto id = builder.add_node({42});
    const auto graph = builder.compile();

    EXPECT_EQ(graph.count(), 1);
    EXPECT_EQ(graph.node(id).value, 42);
}

TEST(csr_digraph_test, multiple_nodes) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({1});
    const auto b = builder.add_node({2});
    const auto c = builder.add_node({3});
    const auto graph = builder.compile();

    EXPECT_EQ(graph.count(), 3);
    EXPECT_EQ(graph.node(a).value, 1);
    EXPECT_EQ(graph.node(b).value, 2);
    EXPECT_EQ(graph.node(c).value, 3);
}

TEST(csr_digraph_test, single_edge) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({1});
    const auto b = builder.add_node({2});
    builder.add_edge(a, b);
    const auto graph = builder.compile();

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 1);
    EXPECT_EQ((*a_out)[0], b);

    const auto b_out = graph.out_edges(b);
    ASSERT_TRUE(b_out.has_value());
    EXPECT_TRUE(b_out->empty());
}

TEST(csr_digraph_test, multiple_out_edges) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(a, d);
    const auto graph = builder.compile();

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 3);
    EXPECT_EQ((*a_out)[0], b);
    EXPECT_EQ((*a_out)[1], c);
    EXPECT_EQ((*a_out)[2], d);
}

TEST(csr_digraph_test, diamond_graph) {
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
