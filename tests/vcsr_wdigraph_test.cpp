#include <gtest/gtest.h>
#include "../include/vcsr.h"
#include "global.h"

TEST(vcsr_wdigraph_test, empty_graph) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
    const auto graph = builder.compile();

    EXPECT_EQ(graph.node_count(), 0);
}

TEST(vcsr_wdigraph_test, single_node) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
    const auto id = builder.add_node({42});
    const auto graph = builder.compile();

    EXPECT_EQ(graph.node_count(), 1);
    EXPECT_EQ(graph.node(id).value, 42);
}

TEST(vcsr_wdigraph_test, multiple_nodes) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
    const auto a = builder.add_node({1});
    const auto b = builder.add_node({2});
    const auto c = builder.add_node({3});
    const auto graph = builder.compile();

    EXPECT_EQ(graph.node_count(), 3);
    EXPECT_EQ(graph.node(a).value, 1);
    EXPECT_EQ(graph.node(b).value, 2);
    EXPECT_EQ(graph.node(c).value, 3);
}

TEST(vcsr_wdigraph_test, single_edge) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
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

TEST(vcsr_wdigraph_test, multiple_out_edges) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
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

TEST(vcsr_wdigraph_test, diamond_graph) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
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

TEST(vcsr_wdigraph_test, out_of_bounds) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
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

TEST(vcsr_wdigraph_test, in_edges) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
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
    EXPECT_EQ((*c_in)[0], a);
    EXPECT_EQ((*c_in)[1], b);

    auto c_in_w = graph.in_weights(c);
    ASSERT_TRUE(c_in_w.has_value());
    ASSERT_EQ(c_in_w->size(), 2);
    EXPECT_EQ((*c_in_w)[0], 20);
    EXPECT_EQ((*c_in_w)[1], 30);
}

TEST(vcsr_wdigraph_test, is_acyclic) {
    // Acyclic
    dagpp::vcsr::wdigraph_builder<test_node, int> b1;
    const auto a = b1.add_node({0});
    const auto b = b1.add_node({1});
    const auto c = b1.add_node({2});
    b1.add_edge(a, b, 1);
    b1.add_edge(b, c, 1);
    const auto g1 = b1.compile();
    EXPECT_TRUE(g1.is_acyclic());

    // Cyclic
    dagpp::vcsr::wdigraph_builder<test_node, int> b2;
    b2.add_node({0});
    b2.add_node({1});
    b2.add_node({2});
    b2.add_edge(0, 1, 1);
    b2.add_edge(1, 2, 1);
    b2.add_edge(2, 0, 1);
    const auto g2 = b2.compile();
    EXPECT_FALSE(g2.is_acyclic());
    
    // Empty
    dagpp::vcsr::wdigraph_builder<test_node, int> b3;
    const auto g3 = b3.compile();
    EXPECT_TRUE(g3.is_acyclic());
}

TEST(vcsr_wdigraph_test, dynamic_add_nodes_and_edges) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
    auto graph = builder.compile();

    const auto a = graph.add_node({10});
    const auto b = graph.add_node({20});
    const auto c = graph.add_node({30});

    EXPECT_EQ(graph.node_count(), 3);

    graph.add_edge(a, b, 100);
    graph.add_edge(b, c, 200);

    auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 1);
    EXPECT_EQ((*a_out)[0], b);
    
    auto a_out_w = graph.out_weights(a);
    ASSERT_TRUE(a_out_w.has_value());
    ASSERT_EQ(a_out_w->size(), 1);
    EXPECT_EQ((*a_out_w)[0], 100);

    auto c_in = graph.in_edges(c);
    ASSERT_TRUE(c_in.has_value());
    ASSERT_EQ(c_in->size(), 1);
    EXPECT_EQ((*c_in)[0], b);
    
    auto c_in_w = graph.in_weights(c);
    ASSERT_TRUE(c_in_w.has_value());
    ASSERT_EQ(c_in_w->size(), 1);
    EXPECT_EQ((*c_in_w)[0], 200);
}

TEST(vcsr_wdigraph_test, dynamic_rebalance_trigger) {
    dagpp::vcsr::wdigraph_builder<test_node, int> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    auto graph = builder.compile(1.0); // Very dense graph to force rebalance/resize soon

    // Add many edges to trigger insertions and potentially resize
    for (int i = 0; i < 20; ++i) {
        const auto new_node = graph.add_node({i + 2});
        graph.add_edge(a, new_node, i * 10);
    }

    auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 20);

    auto a_out_w = graph.out_weights(a);
    ASSERT_TRUE(a_out_w.has_value());
    ASSERT_EQ(a_out_w->size(), 20);

    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ((*a_out_w)[i], i * 10);
    }
}
