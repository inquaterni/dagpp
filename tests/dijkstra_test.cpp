#include <gtest/gtest.h>
#include "../include/csr.h"
#include "../include/dijkstra.h"
#include "global.h"

TEST(dijkstra_test, simple_path) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    builder.add_node({});
    builder.add_node({1});
    builder.add_node({2});
    builder.add_edge(0, 1, 10);
    builder.add_edge(1, 2, 20);
    builder.add_edge(0, 2, 50);
    const auto graph = builder.compile();

    auto [distances, previous] = dagpp::dijkstra(graph, 0);
    EXPECT_EQ(distances.size(), 3);
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 10);
    EXPECT_EQ(distances[2], 30);
    
    EXPECT_EQ(previous[0], std::numeric_limits<dagpp::nodeid_t>::max());
    EXPECT_EQ(previous[1], 0);
    EXPECT_EQ(previous[2], 1);
}

TEST(dijkstra_test, unreachable_node) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    builder.add_node({0});
    builder.add_node({1});
    const auto graph = builder.compile();

    auto [distances, previous] = dagpp::dijkstra(graph, 0);
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], std::numeric_limits<int>::max());
}

TEST(dijkstra_test, cyclic_graph) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    builder.add_node({0});
    builder.add_node({1});
    builder.add_node({2});
    builder.add_edge(0, 1, 5);
    builder.add_edge(1, 2, 5);
    builder.add_edge(2, 0, 1);
    builder.add_edge(0, 2, 4);
    const auto graph = builder.compile();

    auto [distances, previous] = dagpp::dijkstra(graph, 0);
    EXPECT_EQ(distances[0], 0);
    EXPECT_EQ(distances[1], 5);
    EXPECT_EQ(distances[2], 4);
    
    EXPECT_EQ(previous[0], std::numeric_limits<dagpp::nodeid_t>::max());
    EXPECT_EQ(previous[1], 0);
    EXPECT_EQ(previous[2], 0);
}

TEST(dijkstra_test, multiple_paths) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    builder.add_node({0});
    builder.add_node({1});
    builder.add_node({2});
    builder.add_node({3});
    builder.add_edge(0, 1, 1);
    builder.add_edge(1, 2, 2);
    builder.add_edge(2, 3, 1);
    builder.add_edge(0, 3, 5);
    const auto graph = builder.compile();

    auto [distances, previous] = dagpp::dijkstra(graph, 0);
    EXPECT_EQ(distances[3], 4);
    EXPECT_EQ(previous[3], 2);
}

TEST(dijkstra_test, empty_graph) {
    dagpp::csr::wdigraph_builder<test_node, int> builder;
    const auto graph = builder.compile();

    auto [distances, previous] = dagpp::dijkstra(graph, 0);
    EXPECT_TRUE(distances.empty());
    EXPECT_TRUE(previous.empty());
}
