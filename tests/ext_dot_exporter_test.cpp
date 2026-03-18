//
// Created by inquaterni on 3/17/26.
//
#include <gtest/gtest.h>
#include "../include/csr.h"
#include "../include/dot.h"
#include "global.h"

TEST(dot_exporter_test, empty_csr_graph_outbound) {
    dagpp::csr::digraph_builder<test_node> builder;
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

    in.close();
    std::filesystem::remove("test.dot");
}
TEST(dot_exporter_test, empty_csr_graph_inbound) {
    dagpp::csr::digraph_builder<test_node> builder;
    const auto graph = builder.compile<dagpp::ext::dot_exporter>();
    auto out = std::ofstream("test.dot");
    ASSERT_TRUE(out.is_open());

    graph.to_dot<dagpp::inbound>([] (const std::size_t i, const test_node &node){
        return std::format("\tn{} [label=\"{}\"];\n", i, node.value);
    }, out);

    out.close();

    ASSERT_TRUE(std::filesystem::exists("test.dot"));
    std::ifstream in("test.dot");
    ASSERT_TRUE(in.is_open());

    const std::string content((std::istreambuf_iterator(in)),
                               std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "digraph G {\n}\n");

    in.close();
    std::filesystem::remove("test.dot");
}

TEST(dot_exporter_test, diamond_csr_graph_outbound) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::csr::digraph_builder<test_node> builder;
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

    in.close();
    std::filesystem::remove("test.dot");
}
TEST(dot_exporter_test, diamond_csr_graph_inbound) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::csr::digraph_builder<test_node> builder;
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

    graph.to_dot<dagpp::inbound>([] (const std::size_t i, const test_node &node){
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
                       "\tn1 [label=\"1\"];\n"
                       "\tn1 -> n0;\n"
                       "\tn2 [label=\"2\"];\n"
                       "\tn2 -> n0;\n"
                       "\tn3 [label=\"3\"];\n"
                       "\tn3 -> n1;\n\tn3 -> n2;\n"
                       "}\n");

    in.close();
    std::filesystem::remove("test.dot");
}

TEST(dot_exporter_mut_test, empty_graph_outbound) {
    dagpp::digraph<test_node, dagpp::ext::dot_exporter> graph;
    auto out = std::ofstream("test.dot");
    ASSERT_TRUE(out.is_open());

    graph.to_dot([] (const std::size_t i, const test_node &node){
        return std::format("\tn{} [label=\"{}\"];\n", i, node.value);
    }, out);

    out.close();

    ASSERT_TRUE(std::filesystem::exists("test.dot"));
    std::ifstream in("test.dot");
    ASSERT_TRUE(in.is_open());

    const std::string content((std::istreambuf_iterator<char>(in)),
                               std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "digraph G {\n}\n");

    in.close();
    std::filesystem::remove("test.dot");
}
TEST(dot_exporter_mut_test, empty_graph_inbound) {
    dagpp::digraph<test_node, dagpp::ext::dot_exporter> graph;
    auto out = std::ofstream("test.dot");
    ASSERT_TRUE(out.is_open());

    graph.to_dot<dagpp::inbound>([] (const std::size_t i, const test_node &node){
        return std::format("\tn{} [label=\"{}\"];\n", i, node.value);
    }, out);

    out.close();

    ASSERT_TRUE(std::filesystem::exists("test.dot"));
    std::ifstream in("test.dot");
    ASSERT_TRUE(in.is_open());

    const std::string content((std::istreambuf_iterator<char>(in)),
                               std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "digraph G {\n}\n");

    in.close();
    std::filesystem::remove("test.dot");
}

TEST(dot_exporter_mut_test, diamond_graph_outbound) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::digraph<test_node, dagpp::ext::dot_exporter> graph;
    const auto a = graph.add_node({});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    const auto d = graph.add_node({3});
    graph.add_edge(a, b);
    graph.add_edge(a, c);
    graph.add_edge(b, d);
    graph.add_edge(c, d);
    auto out = std::ofstream("test.dot");
    ASSERT_TRUE(out.is_open());

    graph.to_dot([] (const std::size_t i, const test_node &node){
        return std::format("\tn{} [label=\"{}\"];\n", i, node.value);
    }, out);

    out.close();

    ASSERT_TRUE(std::filesystem::exists("test.dot"));
    std::ifstream in("test.dot");
    ASSERT_TRUE(in.is_open());

    const std::string content((std::istreambuf_iterator<char>(in)),
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

    in.close();
    std::filesystem::remove("test.dot");
}
TEST(dot_exporter_mut_test, diamond_graph_inbound) {
    // A -> B, A -> C, B -> D, C -> D
    dagpp::digraph<test_node, dagpp::ext::dot_exporter> graph;
    const auto a = graph.add_node({});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});
    const auto d = graph.add_node({3});
    graph.add_edge(a, b);
    graph.add_edge(a, c);
    graph.add_edge(b, d);
    graph.add_edge(c, d);
    auto out = std::ofstream("test.dot");
    ASSERT_TRUE(out.is_open());

    graph.to_dot<dagpp::inbound>([] (const std::size_t i, const test_node &node){
        return std::format("\tn{} [label=\"{}\"];\n", i, node.value);
    }, out);

    out.close();

    ASSERT_TRUE(std::filesystem::exists("test.dot"));
    std::ifstream in("test.dot");
    ASSERT_TRUE(in.is_open());

    const std::string content((std::istreambuf_iterator<char>(in)),
                               std::istreambuf_iterator<char>());
    EXPECT_EQ(content, "digraph G {\n"
                       "\tn0 [label=\"0\"];\n"
                       "\tn1 [label=\"1\"];\n"
                       "\tn1 -> n0;\n"
                       "\tn2 [label=\"2\"];\n"
                       "\tn2 -> n0;\n"
                       "\tn3 [label=\"3\"];\n"
                       "\tn3 -> n1;\n\tn3 -> n2;\n"
                       "}\n");

    in.close();
    std::filesystem::remove("test.dot");
}

struct MockErrorGraphDot : public dagpp::ext::dot_exporter {
    using node_type = test_node;
    using size_type = std::size_t;
    std::size_t count() const { return 1; }
    test_node node(dagpp::nodeid_t) const { return {0}; }
    std::expected<std::span<const dagpp::nodeid_t>, std::string> out_edges(dagpp::nodeid_t) const {
        return std::unexpected{"Mock error"};
    }
    std::expected<std::span<const dagpp::nodeid_t>, std::string> in_edges(dagpp::nodeid_t) const {
        return std::unexpected{"Mock error"};
    }
    bool is_acyclic() const { return true; }
};

TEST(dot_exporter_mut_test, node_error) {
    MockErrorGraphDot g;
    auto out = std::ofstream("test_err.dot");
    g.to_dot([] (const std::size_t i, const test_node &n){
        return std::format("\tn{} [label=\"{}\"];\n", i, n.value);
    }, out);
    out.close();

    std::ifstream in("test_err.dot");
    const std::string content((std::istreambuf_iterator<char>(in)),
                               std::istreambuf_iterator<char>());
    // It should skip the edges writing
    EXPECT_EQ(content, "digraph G {\n\tn0 [label=\"0\"];\n}\n");
    in.close();
    std::filesystem::remove("test_err.dot");
}