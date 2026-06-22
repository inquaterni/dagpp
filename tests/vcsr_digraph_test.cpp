//
// Created by inquaterni on 6/16/26.
//

#include <gtest/gtest.h>
#include "../include/vcsr.h"
#include "../include/detail.h"
#include "global.h"
#include <random>

static constexpr double density = 0.8;

TEST(vcsr_digraph_test, empty_graph) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto graph = builder.compile(density);

    EXPECT_EQ(graph.node_count(), 0);
}

TEST(vcsr_digraph_test, single_node) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto id = builder.emplace_node(42);
    const auto graph = builder.compile(density);

    EXPECT_EQ(graph.node_count(), 1);
    EXPECT_EQ(graph.node(id).value, 42);
}

TEST(vcsr_digraph_test, multiple_nodes) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({1});
    const auto b = builder.add_node({2});
    const auto c = builder.add_node({3});
    const auto graph = builder.compile(density);

    EXPECT_EQ(graph.node_count(), 3);
    EXPECT_EQ(graph.node(a).value, 1);
    EXPECT_EQ(graph.node(b).value, 2);
    EXPECT_EQ(graph.node(c).value, 3);
}

TEST(vcsr_digraph_test, single_edge) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({1});
    const auto b = builder.add_node({2});
    builder.add_edge(a, b);
    const auto graph = builder.compile(density);

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 1);
    EXPECT_EQ((*a_out)[0], b);

    const auto b_out = graph.out_edges(b);
    ASSERT_TRUE(b_out.has_value());
    EXPECT_TRUE(b_out->empty());
}

TEST(vcsr_digraph_test, multiple_out_edges) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(a, d);
    const auto graph = builder.compile(density);

    const auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 3);
    EXPECT_EQ((*a_out)[0], b);
    EXPECT_EQ((*a_out)[1], c);
    EXPECT_EQ((*a_out)[2], d);
}

TEST(vcsr_digraph_test, diamond_graph) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, d);
    builder.add_edge(c, d);
    const auto graph = builder.compile(density);

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

TEST(vcsr_digraph_test, lvalue_add_node_and_reserve) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    builder.reserve_nodes(2);
    builder.reserve_edges(1);
    test_node n1{10};
    test_node n2{20};
    builder.emplace_node(n1);
    builder.emplace_node(n2);
    builder.add_edge(0, 1);
    const auto graph = builder.compile(density);
    EXPECT_EQ(graph.node_count(), 2);
}

TEST(vcsr_digraph_test, out_of_bounds) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    builder.add_node({1});
    const auto graph = builder.compile(density);

    auto out = graph.out_edges(1);
    EXPECT_FALSE(out.has_value());
    EXPECT_EQ(out.error(), "Index is out of range.");

    auto in = graph.in_edges(1);
    EXPECT_FALSE(in.has_value());
    EXPECT_EQ(in.error(), "Index is out of range.");
}

TEST(vcsr_digraph_test, in_edges) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, c);
    const auto graph = builder.compile(density);

    auto a_in = graph.in_edges(a);
    ASSERT_TRUE(a_in.has_value());
    EXPECT_TRUE(a_in->empty());

    auto b_in = graph.in_edges(b);
    ASSERT_TRUE(b_in.has_value());
    ASSERT_EQ(b_in->size(), 1);
    EXPECT_EQ((*b_in)[0], a);

    auto c_in = graph.in_edges(c);
    ASSERT_TRUE(c_in.has_value());
    ASSERT_EQ(c_in->size(), 2);
    EXPECT_EQ((*c_in)[0], a);
    EXPECT_EQ((*c_in)[1], b);
}

TEST(vcsr_digraph_test, is_acyclic) {
    // Acyclic
    dagpp::vcsr::digraph_builder<test_node> b1;
    const auto a = b1.add_node({0});
    const auto b = b1.add_node({1});
    const auto c = b1.add_node({2});
    b1.add_edge(a, b);
    b1.add_edge(b, c);
    const auto g1 = b1.compile(density);
    EXPECT_TRUE(g1.is_acyclic());

    // Cyclic
    dagpp::vcsr::digraph_builder<test_node> b2;
    b2.add_node({0});
    b2.add_node({1});
    b2.add_node({2});
    b2.add_edge(0, 1);
    b2.add_edge(1, 2);
    b2.add_edge(2, 0);
    const auto g2 = b2.compile(density);
    EXPECT_FALSE(g2.is_acyclic());

    // Empty
    dagpp::vcsr::digraph_builder<test_node> b3;
    const auto g3 = b3.compile(density);
    EXPECT_TRUE(g3.is_acyclic());
}

TEST(vcsr_digraph_test, self_loop_cyclic) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({0});
    builder.add_edge(a, a);
    const auto graph = builder.compile(density);

    EXPECT_FALSE(graph.is_acyclic());

    auto a_out = graph.out_edges(a);
    ASSERT_TRUE(a_out.has_value());
    ASSERT_EQ(a_out->size(), 1);
    EXPECT_EQ((*a_out)[0], a);
}

TEST(vcsr_digraph_test, disconnected_components) {
    // A -> B, and C -> D (Two separate islands)
    dagpp::vcsr::digraph_builder<test_node> builder;
    const auto a = builder.add_node({10});
    const auto b = builder.add_node({20});
    const auto c = builder.add_node({30});
    const auto d = builder.add_node({40});

    builder.add_edge(a, b);
    builder.add_edge(c, d);
    const auto graph = builder.compile(density);

    EXPECT_TRUE(graph.is_acyclic());

    auto b_in = graph.in_edges(b);
    ASSERT_TRUE(b_in.has_value());
    EXPECT_EQ((*b_in)[0], a);

    auto d_in = graph.in_edges(d);
    ASSERT_TRUE(d_in.has_value());
    EXPECT_EQ((*d_in)[0], c);

    EXPECT_TRUE(graph.out_edges(b)->empty());
    EXPECT_TRUE(graph.out_edges(d)->empty());
}

TEST(vcsr_digraph_test, dynamic_add_node_fast_path) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    builder.add_node({1});
    builder.add_node({2});
    builder.add_node({3});
    auto graph = builder.compile(density);

    EXPECT_EQ(graph.node_count(), 3);

    const auto new_id = graph.emplace_node(4);

    EXPECT_EQ(new_id, 3);
    EXPECT_EQ(graph.node_count(), 4);
    EXPECT_EQ(graph.node(new_id).value, 4);

    EXPECT_TRUE(graph.out_edges(new_id)->empty());
    EXPECT_TRUE(graph.in_edges(new_id)->empty());
}

TEST(vcsr_digraph_test, dynamic_add_node_geometric_expansion) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    builder.add_node({1});
    builder.add_node({2});
    builder.add_node({3});
    auto graph = builder.compile(density);

    graph.emplace_node(4);
    EXPECT_EQ(graph.node_count(), 4);

    const auto expanded_id = graph.add_node(test_node{5});

    EXPECT_EQ(expanded_id, 4);
    EXPECT_EQ(graph.node_count(), 5);
    EXPECT_EQ(graph.node(expanded_id).value, 5);

    EXPECT_EQ(graph.node(0).value, 1);
    EXPECT_EQ(graph.node(3).value, 4);

    EXPECT_TRUE(graph.in_edges(expanded_id)->empty());
}

TEST(vcsr_digraph_test, dynamic_add_node_with_edges_and_expansion) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    builder.add_node({0});
    builder.add_node({1});
    builder.add_node({2});
    builder.add_edge(0, 1);
    builder.add_edge(1, 2);
    builder.add_edge(2, 0);

    auto graph = builder.compile(density);

    // Add nodes until expansion happens
    graph.emplace_node(3);
    graph.add_edge(3, 0);
    graph.add_edge(2, 3);

    // Emplacing 5th node triggers expansion
    graph.emplace_node(4);
    graph.add_edge(4, 1);
    graph.add_edge(3, 4);

    // Verify all old and new edges
    auto e0 = graph.out_edges(0);
    ASSERT_TRUE(e0.has_value());
    ASSERT_EQ(e0->size(), 1);
    EXPECT_EQ((*e0)[0], 1);

    auto e1 = graph.out_edges(1);
    ASSERT_TRUE(e1.has_value());
    ASSERT_EQ(e1->size(), 1);
    EXPECT_EQ((*e1)[0], 2);

    auto e2 = graph.out_edges(2);
    ASSERT_TRUE(e2.has_value());
    ASSERT_EQ(e2->size(), 2); // 2->0, 2->3

    auto e3 = graph.out_edges(3);
    ASSERT_TRUE(e3.has_value());
    ASSERT_EQ(e3->size(), 2); // 3->0, 3->4

    auto e4 = graph.out_edges(4);
    ASSERT_TRUE(e4.has_value());
    ASSERT_EQ(e4->size(), 1); // 4->1

    auto in1 = graph.in_edges(1);
    ASSERT_TRUE(in1.has_value());
    ASSERT_EQ(in1->size(), 2); // 0->1, 4->1
}

TEST(vcsr_digraph_test, stress_test_dynamic_additions) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    auto graph = builder.compile();

    constexpr int num_nodes = 2000;

    std::vector<std::vector<std::size_t>> expected_out(num_nodes);
    std::vector<std::vector<std::size_t>> expected_in(num_nodes);

    std::mt19937 gen(42);

    // Interleave node additions and edge additions
    for (int i = 0; i < num_nodes; ++i) {
        graph.emplace_node(i);

        // Add some edges to previously existing nodes (or the new node)
        if (i > 0) {
            std::uniform_int_distribution dis(0, i);
            int edges_to_add = std::min(i, 20);
            for (int j = 0; j < edges_to_add; ++j) {
                int src = dis(gen);
                int dst = dis(gen);
                graph.add_edge(src, dst);
                expected_out[src].push_back(dst);
                expected_in[dst].push_back(src);
            }
        }
    }

    // Verify all edges
    for (int i = 0; i < num_nodes; ++i) {
        std::ranges::sort(expected_out[i]);
        std::ranges::sort(expected_in[i]);

        auto out_res = graph.out_edges(i);
        ASSERT_TRUE(out_res.has_value());
        std::vector actual_out(out_res->begin(), out_res->end());
        std::ranges::sort(actual_out);
        ASSERT_EQ(actual_out, expected_out[i]) << "Mismatch out-edges for node " << i;

        auto in_res = graph.in_edges(i);
        ASSERT_TRUE(in_res.has_value());
        std::vector actual_in(in_res->begin(), in_res->end());
        std::ranges::sort(actual_in);
        ASSERT_EQ(actual_in, expected_in[i]) << "Mismatch in-edges for node " << i;
    }
}

TEST(vcsr_digraph_test, reserve_nodes_and_edges) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    // Test reserve_nodes and reserve_edges on the builder
    builder.reserve_nodes(500);
    builder.reserve_edges(1000);
    
    auto graph = builder.compile(density);
    
    // Add nodes to ensure nothing was broken by reservation
    auto id1 = graph.emplace_node(10);
    auto id2 = graph.emplace_node(20);
    graph.add_edge(id1, id2);
    
    EXPECT_EQ(graph.node_count(), 2);
    EXPECT_EQ(graph.edge_count(), 1);
}

TEST(vcsr_digraph_test, insert_forces_left_shift) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    auto graph = builder.compile(density);
    
    // We want to force a left shift in the PMA insert() method.
    // Left shifts occur when the right side of an insertion point is full
    // but there is free space to the left.
    // The easiest way to force this is to add several nodes, 
    // fill the edges of the last node in the segment, and then add an edge.
    
    std::vector<std::size_t> ids;
    for (int i = 0; i < 10; ++i) {
        ids.push_back(graph.emplace_node(i));
    }
    
    // Add enough edges to node 8 to push node 9 to the right edge of the segment.
    // Then add an edge to node 9, which will have no right_free_slot and will be forced to left-shift node 8.
    // Note: The specific internal layout depends on segment size, but adding a lot of edges
    // localized to the end of the node list will inevitably trigger left shifts or rebalances.
    
    for (int i = 0; i < 5; ++i) {
        graph.add_edge(ids[8], ids[i]);
    }
    
    // Now add an edge to 9. Since it's at the end of the node array, it might be right at the boundary.
    // If we add edges to 9, it expands.
    for (int i = 0; i < 5; ++i) {
        graph.add_edge(ids[9], ids[i]);
    }
    
    // Keep thrashing the localized area to force left/right shifts and rebalances
    for (int i = 0; i < 15; ++i) {
        graph.add_edge(ids[7], ids[i % 5]);
    }
    
    // Verify edge count
    EXPECT_EQ(graph.edge_count(), 25);
}

TEST(vcsr_digraph_test, rebalance_wrapper_and_weighted) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    auto graph = builder.compile(0.9); // High density to easily trigger rebalance
    
    std::vector<std::size_t> ids;
    for (int i = 0; i < 50; ++i) {
        ids.push_back(graph.emplace_node(i));
    }
    
    // Adding many edges to a single node creates a localized density spike.
    // This will cause its segment to exceed `up_height` density, triggering
    // `rebalance_wrapper` and `rebalance_weighted` (and update_segment_edge_total).
    for (int i = 0; i < 40; ++i) {
        graph.add_edge(ids[25], ids[i]);
    }
    
    // Verify integrity
    auto out_25 = graph.out_edges(ids[25]);
    ASSERT_TRUE(out_25.has_value());
    EXPECT_EQ(out_25->size(), 40);
}

TEST(vcsr_digraph_test, node_id_consistency_across_expansion) {
    dagpp::vcsr::digraph_builder<test_node> builder;
    auto graph = builder.compile(density);

    std::vector<std::size_t> ids;
    for (int i = 0; i < 2000; ++i) {
        // We set the payload value to a multiple of i to verify it later
        auto id = graph.emplace_node(i * 10);
        ids.push_back(id);
        
        // Also verify that the returned ID is just sequentially assigned
        EXPECT_EQ(id, static_cast<std::size_t>(i));
    }

    // Now, verify that all original IDs still map to the same payload
    // after dozens of expansions have happened.
    for (int i = 0; i < 2000; ++i) {
        auto expected_payload = i * 10;
        EXPECT_EQ(graph.node(ids[i]).value, expected_payload) << "Mismatch payload for node " << i;
    }
}