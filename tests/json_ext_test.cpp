//
// Created by inquaterni on 3/17/26.
//
#include <filesystem>
#include <fstream>
#include "../include/digraph.h"
#include "gtest/gtest.h"


struct my_node {
    int id;
};

struct json_exporter {

    template<typename TSelf>
    void export_json(this const TSelf &self, std::ofstream &out) {
        out << "{\n  \"nodes\": [\n";
        for (std::size_t i = 0; i < self.count(); ++i) {
            // Assuming the node has an 'id' field
            out << "    { \"id\": " << i
                      << ", \"value\": " << self.node(i).id << " }";
            if (i < self.count() - 1) out << ",";
            out << "\n";
        }
        out << "  ],\n  \"edges\": [\n";
        bool first = true;
        for (std::size_t i = 0; i < self.count(); ++i) {
            auto edges = self.out_edges(i);
            if (!edges) continue;
            for (auto target : *edges) {
                if (!first) out << ",\n";
                out << "    { \"source\": " << i << ", \"target\": " << target << " }";
                first = false;
            }
        }
        out << "\n  ]\n}\n";
    }
};

TEST(json_ext_test, full_program_test) {
    dagpp::digraph_builder<my_node> builder;

    const auto a = builder.add_node({10});
    const auto b = builder.add_node({20});
    const auto c = builder.add_node({30});

    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, c);

    // Compile graph with your custom extension
    const auto graph = builder.compile<json_exporter>();
    // You can also use multiple extensions
    // const auto graph = builder.compile<json_exporter, dagpp::ext::dot_exporter>();


    // Export graph to JSON
    std::ofstream out("graph.json");
    ASSERT_TRUE(out.is_open());
    graph.export_json(out);
    ASSERT_TRUE(std::filesystem::exists("graph.json"));

    std::filesystem::remove("graph.json");
}