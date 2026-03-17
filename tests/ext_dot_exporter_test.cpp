//
// Created by inquaterni on 3/17/26.
//
#include <gtest/gtest.h>
#include "../include/digraph.h"
#include "../include/ext.h"
#include "global.h"

TEST(dot_exporter_test, empty_graph) {
    dagpp::digraph_builder<test_node> builder;
    const auto graph = builder.compile<dagpp::ext::dot_exporter>();
    auto out = std::ofstream("test.dot");
    ASSERT_TRUE(out.is_open());

    graph.to_dot([] (const std::size_t i, const test_node &node){
        return std::format("\tn{} [label=\"{}\"];\n", i, node.value);
    }, out);

    out.close();

    ASSERT_TRUE(std::filesystem::exists("test.dot"));
    std::ifstream in("test.dot");
    ASSERT_TRUE(in.is_open());

    const std::string content((std::istreambuf_iterator(in)),
                               std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "digraph G {\n}\n");

    std::filesystem::remove("test.dot");
}
TEST(dot_exporter_test, diamond_graph) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::digraph_builder<test_node> builder;
    const auto a = builder.add_node({});
    const auto b = builder.add_node({1});
    const auto c = builder.add_node({2});
    const auto d = builder.add_node({3});
    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, d);
    builder.add_edge(c, d);
    const auto graph = builder.compile<dagpp::ext::dot_exporter>();
    auto out = std::ofstream("test.dot");
    ASSERT_TRUE(out.is_open());

    graph.to_dot([] (const std::size_t i, const test_node &node){
        return std::format("\tn{} [label=\"{}\"];\n", i, node.value);
    }, out);

    out.close();

    ASSERT_TRUE(std::filesystem::exists("test.dot"));
    std::ifstream in("test.dot");
    ASSERT_TRUE(in.is_open());

    const std::string content((std::istreambuf_iterator(in)),
                               std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "digraph G {\n"
                       "\tn0 [label=\"0\"];\n"
                       "\tn0 -> n1;\n"
                       "\tn0 -> n2;\n"
                       "\tn1 [label=\"1\"];\n"
                       "\tn1 -> n3;\n"
                       "\tn2 [label=\"2\"];\n"
                       "\tn2 -> n3;\n"
                       "\tn3 [label=\"3\"];\n"
                       "}\n");

    std::filesystem::remove("test.dot");
}