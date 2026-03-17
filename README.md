# dag++

A lightweight, fast, and cache-friendly C++23 directed graph library.

## Features

- **Builder Pattern & CSR:** Build graphs intuitively with `digraph_builder` and compile them into an immutable, highly cache-friendly Compressed Sparse Row (CSR) representation.
- **Zero-Cost Mixin Extensions:** Enhance the core `digraph` type using the "Deducing This" (CRTP-like) pattern. Opt-in to features (like `.to_dot()`) without runtime bloat, vtables, or graph traversal penalties.
- **Topological Sort & Cycle Detection:** Built-in Kahn's algorithm implementation.
- **Lightweight & Standard:** `std::expected` for error handling, `std::span` for fast edge views. Compatible with `-fno-exceptions`.

## Quick Start

## Build Instructions

Requires a C++23 compliant compiler and CMake 3.31+.

```bash
cmake -S . -B build
cmake --build build
```

**Testing** (requires GoogleTest, which is fetched automatically):
```bash
cd build && ctest --output-on-failure
```


### Basic Usage

```cpp
#include <iostream>
#include "include/digraph.h"
#include "include/topo_sort.h"
#include "include/ext.h" // Optional: for DOT export


// Node data
// NOTE: as of now node type MUST be trivial.
struct my_node {
    int id;
};

int main() {
    dagpp::digraph_builder<my_node> builder;

    auto a = builder.add_node({10});
    auto b = builder.add_node({20});
    auto c = builder.add_node({30});

    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, c);

    // Compile graph with DOT export extension
    const auto graph = builder.compile<dagpp::ext::dot_exporter>();

    if (auto out = graph.out_edges(a); out.has_value()) {
        std::cout << "Node A has " << out->size() << " outgoing edges.\n";
    }

    if (auto order = dagpp::topo_sort(graph); order.has_value()) {
        std::cout << "Graph is a DAG!\n";
    }

    std::ofstream out("graph.dot");
    graph.to_dot([](const std::size_t index, const my_node& node) {
        return std::format("\tn{} [label=\"{}\"];\n", index, node.id);
    }, out);
    out.close();

    return 0;
}
```

### Extensions

```cpp
#include <fstream>
#include "include/digraph.h"
#include "include/ext.h"


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

int main() {
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
    graph.export_json(out);
    out.close();

    return 0;
}

```
