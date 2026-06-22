#include <gtest/gtest.h>
#include "../include/digraph.h"
#include "../include/dijkstra.h"
#include "global.h"

TEST(mut_wdigraph_test, empty_graph) {
    const dagpp::wdigraph<test_node, int> graph;
    EXPECT_EQ(graph.node_count(), 0);
}

TEST(mut_wdigraph_test, single_node) {
    dagpp::wdigraph<test_node, int> graph;
    const auto id = graph.add_node({42});
    EXPECT_EQ(graph.node_count(), 1);
    EXPECT_EQ(graph.node(id).value, 42);
}

TEST(mut_wdigraph_test, multiple_nodes) {
    dagpp::wdigraph<test_node, int> graph;
    const auto a = graph.add_node({1});
    const auto b = graph.add_node({2});
    const auto c = graph.add_node({3});
    EXPECT_EQ(graph.node_count(), 3);
    EXPECT_EQ(graph.node(a).value, 1);
    EXPECT_EQ(graph.node(b).value, 2);
    EXPECT_EQ(graph.node(c).value, 3);
}

TEST(mut_wdigraph_test, single_edge) {
    dagpp::wdigraph<test_node, int> graph;
    const auto a = graph.add_node({1});
    const auto b = graph.add_node({2});
    graph.add_edge(a, b, 10);

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 1);
    EXPECT_EQ((*a_out)[0], b);

    const auto a_out_w = graph.out_weights(a);
    ASSERT_TRUE(a_out_w.has_value());
    ASSERT_EQ(a_out_w->size(), 1);
    EXPECT_EQ((*a_out_w)[0], 10);

    const auto b_out = graph.out_edges(b);
    ASSERT_TRUE(b_out.has_value());
    EXPECT_TRUE(b_out->empty());

    const auto b_out_w = graph.out_weights(b);
    ASSERT_TRUE(b_out_w.has_value());
    EXPECT_TRUE(b_out_w->empty());
}

TEST(mut_wdigraph_test, multiple_out_edges) {
    dagpp::wdigraph<test_node, int> graph;
    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    const auto d = graph.add_node({3});
    graph.add_edge(a, b, 10);
    graph.add_edge(a, c, 20);
    graph.add_edge(a, d, 30);

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 3);
    EXPECT_EQ((*a_out)[0], b);
    EXPECT_EQ((*a_out)[1], c);
    EXPECT_EQ((*a_out)[2], d);

    const auto a_out_w = graph.out_weights(a);
    ASSERT_TRUE(a_out_w.has_value());
    ASSERT_EQ(a_out_w->size(), 3);
    EXPECT_EQ((*a_out_w)[0], 10);
    EXPECT_EQ((*a_out_w)[1], 20);
    EXPECT_EQ((*a_out_w)[2], 30);
}

TEST(mut_wdigraph_test, diamond_graph) {
    // A →(1) B, A →(2) C, B →(3) D, C →(4) D
    dagpp::wdigraph<test_node, int> graph;
    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    const auto d = graph.add_node({3});
    graph.add_edge(a, b, 1);
    graph.add_edge(a, c, 2);
    graph.add_edge(b, d, 3);
    graph.add_edge(c, d, 4);

    const auto a_out   = graph.out_edges(a);
    const auto a_out_w = graph.out_weights(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 2);
    EXPECT_EQ((*a_out)[0], b);   EXPECT_EQ((*a_out_w)[0], 1);
    EXPECT_EQ((*a_out)[1], c);   EXPECT_EQ((*a_out_w)[1], 2);

    const auto b_out   = graph.out_edges(b);
    const auto b_out_w = graph.out_weights(b);
    ASSERT_EQ(b_out->size(), 1);
    EXPECT_EQ((*b_out)[0], d);   EXPECT_EQ((*b_out_w)[0], 3);

    const auto c_out   = graph.out_edges(c);
    const auto c_out_w = graph.out_weights(c);
    ASSERT_EQ(c_out->size(), 1);
    EXPECT_EQ((*c_out)[0], d);   EXPECT_EQ((*c_out_w)[0], 4);

    EXPECT_TRUE(graph.out_edges(d)->empty());
    EXPECT_TRUE(graph.out_weights(d)->empty());
}

TEST(mut_wdigraph_test, in_edges) {
    dagpp::wdigraph<test_node, int> graph;
    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    graph.add_edge(a, b, 10);
    graph.add_edge(a, c, 20);
    graph.add_edge(b, c, 30);

    auto a_in = graph.in_edges(a);
    ASSERT_TRUE(a_in.has_value());
    EXPECT_TRUE(a_in->empty());

    auto b_in   = graph.in_edges(b);
    auto b_in_w = graph.in_weights(b);
    ASSERT_EQ(b_in->size(), 1);
    EXPECT_EQ((*b_in)[0], a);
    EXPECT_EQ((*b_in_w)[0], 10);

    auto c_in   = graph.in_edges(c);
    auto c_in_w = graph.in_weights(c);
    ASSERT_EQ(c_in->size(), 2);
    EXPECT_EQ((*c_in)[0], a);   EXPECT_EQ((*c_in_w)[0], 20);
    EXPECT_EQ((*c_in)[1], b);   EXPECT_EQ((*c_in_w)[1], 30);
}

TEST(mut_wdigraph_test, out_of_bounds) {
    dagpp::wdigraph<test_node, int> graph;
    graph.add_node({1});

    auto out = graph.out_edges(1);
    EXPECT_FALSE(out.has_value());
    EXPECT_EQ(out.error(), "Index is out of range or node is invalid.");

    auto in = graph.in_edges(1);
    EXPECT_FALSE(in.has_value());
    EXPECT_EQ(in.error(), "Index is out of range or node is invalid.");

    auto out_w = graph.out_weights(1);
    EXPECT_FALSE(out_w.has_value());
    EXPECT_EQ(out_w.error(), "Index is out of range or node is invalid.");

    auto in_w = graph.in_weights(1);
    EXPECT_FALSE(in_w.has_value());
    EXPECT_EQ(in_w.error(), "Index is out of range or node is invalid.");
}

TEST(mut_wdigraph_test, is_acyclic) {
    // Acyclic chain
    dagpp::wdigraph<test_node, int> g1;
    const auto a = g1.add_node({0});
    const auto b = g1.add_node({1});
    const auto c = g1.add_node({2});
    g1.add_edge(a, b, 1);
    g1.add_edge(b, c, 1);
    EXPECT_TRUE(g1.is_acyclic());

    // Cyclic
    dagpp::wdigraph<test_node, int> g2;
    g2.add_node({0});
    g2.add_node({1});
    g2.add_node({2});
    g2.add_edge(0, 1, 1);
    g2.add_edge(1, 2, 1);
    g2.add_edge(2, 0, 1);
    EXPECT_FALSE(g2.is_acyclic());

    // Empty
    dagpp::wdigraph<test_node, int> g3;
    EXPECT_TRUE(g3.is_acyclic());
}

TEST(mut_wdigraph_test, reserve_nodes_and_rvalue_add_node) {
    dagpp::wdigraph<test_node, int> graph;
    graph.reserve_nodes(3);
    const auto id1 = graph.add_node(test_node{10});
    const auto id2 = graph.add_node(test_node{20});
    EXPECT_EQ(graph.node_count(), 2);

    const dagpp::wdigraph<test_node, int>& cg = graph;
    EXPECT_EQ(cg.node(id1).value, 10);
    EXPECT_EQ(cg.node(id2).value, 20);
}

TEST(mut_wdigraph_test, dijkstra_compatible) {
    // wdigraph satisfies wdirected_graph — verify dijkstra compiles and runs
    dagpp::wdigraph<test_node, int> graph;
    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    graph.add_edge(a, b, 4);
    graph.add_edge(a, c, 1);
    graph.add_edge(c, b, 2);

    const auto result = dagpp::dijkstra(graph, a);
    ASSERT_EQ(result.distances.size(), 3u);
    EXPECT_EQ(result.distances[a], 0);
    EXPECT_EQ(result.distances[c], 1);
    EXPECT_EQ(result.distances[b], 3); // a→c→b = 1+2 < a→b = 4
}
