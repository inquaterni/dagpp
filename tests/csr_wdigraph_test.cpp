#include <gtest/gtest.h>
#include "../include/csr.h"
#include "global.h"

TEST(csr_wdigraph_test, empty_graph) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    const auto graph = builder.compile();

    EXPECT_EQ(graph.count(), 0);
}

TEST(csr_wdigraph_test, single_node) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    const auto id = builder.add_node({42});
    const auto graph = builder.compile();

    EXPECT_EQ(graph.count(), 1);
    EXPECT_EQ(graph.node(id).value, 42);
}

TEST(csr_wdigraph_test, multiple_nodes) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    const auto a = builder.add_node({1});
    const auto b = builder.add_node({2});
    const auto c = builder.add_node({3});
    const auto graph = builder.compile();

    EXPECT_EQ(graph.count(), 3);
    EXPECT_EQ(graph.node(a).value, 1);
    EXPECT_EQ(graph.node(b).value, 2);
    EXPECT_EQ(graph.node(c).value, 3);
}

TEST(csr_wdigraph_test, single_edge) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    const auto a = builder.add_node({1});
    const auto b = builder.add_node({2});
    builder.add_edge(a, b, 10);
    const auto graph = builder.compile();

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 1);
    EXPECT_EQ((*a_out)[0], b);

    const auto a_weights = graph.out_weights(a);
    ASSERT_TRUE(a_weights.has_value());
    ASSERT_EQ(a_weights->size(), 1);
    EXPECT_EQ((*a_weights)[0], 10);

    const auto b_out = graph.out_edges(b);
    ASSERT_TRUE(b_out.has_value());
    EXPECT_TRUE(b_out->empty());

    const auto b_weights = graph.out_weights(b);
    ASSERT_TRUE(b_weights.has_value());
    EXPECT_TRUE(b_weights->empty());
}

TEST(csr_wdigraph_test, multiple_out_edges) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b, 10);
    builder.add_edge(a, c, 20);
    builder.add_edge(a, d, 30);
    const auto graph = builder.compile();

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 3);
    EXPECT_EQ((*a_out)[0], b);
    EXPECT_EQ((*a_out)[1], c);
    EXPECT_EQ((*a_out)[2], d);

    const auto a_weights = graph.out_weights(a);
    ASSERT_TRUE(a_weights.has_value());
    ASSERT_EQ(a_weights->size(), 3);
    EXPECT_EQ((*a_weights)[0], 10);
    EXPECT_EQ((*a_weights)[1], 20);
    EXPECT_EQ((*a_weights)[2], 30);
}

TEST(csr_wdigraph_test, diamond_graph) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b, 1);
    builder.add_edge(a, c, 2);
    builder.add_edge(b, d, 3);
    builder.add_edge(c, d, 4);
    const auto graph = builder.compile();

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 2);
    EXPECT_EQ((*a_out)[0], b);
    EXPECT_EQ((*a_out)[1], c);

    const auto a_weights = graph.out_weights(a);
    ASSERT_TRUE(a_weights.has_value());
    ASSERT_EQ(a_weights->size(), 2);
    EXPECT_EQ((*a_weights)[0], 1);
    EXPECT_EQ((*a_weights)[1], 2);

    const auto b_out = graph.out_edges(b);
    ASSERT_TRUE(b_out.has_value());
    ASSERT_EQ(b_out->size(), 1);
    EXPECT_EQ((*b_out)[0], d);

    const auto b_weights = graph.out_weights(b);
    ASSERT_TRUE(b_weights.has_value());
    ASSERT_EQ(b_weights->size(), 1);
    EXPECT_EQ((*b_weights)[0], 3);

    const auto c_out = graph.out_edges(c);
    ASSERT_TRUE(c_out.has_value());
    ASSERT_EQ(c_out->size(), 1);
    EXPECT_EQ((*c_out)[0], d);

    const auto c_weights = graph.out_weights(c);
    ASSERT_TRUE(c_weights.has_value());
    ASSERT_EQ(c_weights->size(), 1);
    EXPECT_EQ((*c_weights)[0], 4);

    EXPECT_TRUE(graph.out_edges(d)->empty());
    EXPECT_TRUE(graph.out_weights(d)->empty());
}

TEST(csr_wdigraph_test, lvalue_add_node_and_reserve) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    builder.reserve_nodes(2);
    builder.reserve_edges(1);
    test_node n1{10};
    test_node n2{20};
    builder.add_node(n1);
    builder.add_node(n2);
    builder.add_edge(0, 1, 5);
    const auto graph = builder.compile();
    EXPECT_EQ(graph.count(), 2);

    const auto weights = graph.out_weights(0);
    ASSERT_TRUE(weights.has_value());
    ASSERT_EQ(weights->size(), 1);
    EXPECT_EQ((*weights)[0], 5);
}

TEST(csr_wdigraph_test, out_of_bounds) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    builder.add_node({1});
    const auto graph = builder.compile();
    
    auto out = graph.out_edges(1);
    EXPECT_FALSE(out.has_value());
    EXPECT_EQ(out.error(), "Index is out of range.");
    
    auto in = graph.in_edges(1);
    EXPECT_FALSE(in.has_value());
    EXPECT_EQ(in.error(), "Index is out of range.");

    auto out_w = graph.out_weights(1);
    EXPECT_FALSE(out_w.has_value());
    EXPECT_EQ(out_w.error(), "Index is out of range.");
    
    auto in_w = graph.in_weights(1);
    EXPECT_FALSE(in_w.has_value());
    EXPECT_EQ(in_w.error(), "Index is out of range.");
}

TEST(csr_wdigraph_test, in_edges) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    builder.add_edge(a, b, 10);
    builder.add_edge(a, c, 20);
    builder.add_edge(b, c, 30);
    const auto graph = builder.compile();

    auto a_in = graph.in_edges(a);
    ASSERT_TRUE(a_in.has_value());
    EXPECT_TRUE(a_in->empty());

    auto b_in = graph.in_edges(b);
    ASSERT_TRUE(b_in.has_value());
    ASSERT_EQ(b_in->size(), 1);
    EXPECT_EQ((*b_in)[0], a);

    auto b_in_w = graph.in_weights(b);
    ASSERT_TRUE(b_in_w.has_value());
    ASSERT_EQ(b_in_w->size(), 1);
    EXPECT_EQ((*b_in_w)[0], 10);

    auto c_in = graph.in_edges(c);
    ASSERT_TRUE(c_in.has_value());
    ASSERT_EQ(c_in->size(), 2);
    // Order of in-edges depends on insertion order in compile() - A then B
    EXPECT_EQ((*c_in)[0], a);
    EXPECT_EQ((*c_in)[1], b);

    auto c_in_w = graph.in_weights(c);
    ASSERT_TRUE(c_in_w.has_value());
    ASSERT_EQ(c_in_w->size(), 2);
    EXPECT_EQ((*c_in_w)[0], 20);
    EXPECT_EQ((*c_in_w)[1], 30);
}

TEST(csr_wdigraph_test, is_acyclic) {
    // Acyclic
    dagpp::csr::wdigraph_builder<test_node, int> b1;
    const auto a = b1.add_node({0});
    const auto b = b1.add_node({1});
    const auto c = b1.add_node({2});
    b1.add_edge(a, b, 1);
    b1.add_edge(b, c, 1);
    const auto g1 = b1.compile();
    EXPECT_TRUE(g1.is_acyclic());

    // Cyclic
    dagpp::csr::wdigraph_builder<test_node, int> b2;
    b2.add_node({0});
    b2.add_node({1});
    b2.add_node({2});
    b2.add_edge(0, 1, 1);
    b2.add_edge(1, 2, 1);
    b2.add_edge(2, 0, 1);
    const auto g2 = b2.compile();
    EXPECT_FALSE(g2.is_acyclic());
    
    // Empty
    dagpp::csr::wdigraph_builder<test_node, int> b3;
    const auto g3 = b3.compile();
    EXPECT_TRUE(g3.is_acyclic());
}
