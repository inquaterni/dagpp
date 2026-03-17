# dag++

A simple and fast C++23 directed graph library.

![C++23](https://img.shields.io/badge/C%2B%2B-23-blue?logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.31%2B-064F8C?logo=cmake)
![License: MIT](https://img.shields.io/badge/License-MIT-green)
![CI](https://github.com/inquaterni/dagpp/actions/workflows/cmake-multi-platform.yml/badge.svg)
![Coverage](https://img.shields.io/endpoint?url=https://gist.githubusercontent.com/inquaterni/659b74483a5f3c2f917da4678ef3c733/raw/coverage.json)

---

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
- [Build instructions](#build-instructions)
- [Quick start](#quick-start)
  - [Basic usage](#basic-usage)
  - [In-edges & Cycle detection](#in-edges--cycle-detection)
  - [Topological sort](#topological-sort)
  - [DOT export (built-in extension)](#dot-export-built-in-extension)
- [Extensions](#extensions)
  - [JSON exporter custom extension](#json-exporter-custom-extension)
  - [Multiple extensions](#multiple-extensions)
- [API reference](#api-reference)
  - [digraph_builder\<TNode\>](#digraph_buildertnode)
  - [digraph\<TNode, TExtension...\>](#digraphtnode-textension)
  - [topo_sort](#topo_sort)
  - [dagpp::ext::dot_exporter](#dagppextdot_exporter)
- [CMake options](#cmake-options)
- [License](#license)

---

## Features

- Immutable CSR representation with forward and reverse adjacency lists
- `out_edges()` / `in_edges()` return `std::span` views - no extra allocation
- Compile-time extensions via C++23 "Deducing This" (no vtables)
- Topological sort (Kahn's) and `is_acyclic()` included
- `std::expected` for errors; `-fno-exceptions` / `-fno-rtti` compatible
- Header-only

## Architecture

```
digraph_builder<TNode>          (mutable, accumulates nodes & edges)
        │
        │  .compile<Ext1, Ext2, ...>()
        ▼
digraph<TNode, Ext1, Ext2, ...> (immutable CSR, inherits from each Ext)
        ├── .out_edges(id)       → std::expected<std::span<nodeid_t>, string>
        ├── .in_edges(id)        → std::expected<std::span<nodeid_t>, string>
        ├── .node(id)            → const TNode&
        ├── .count()             → std::size_t
        ├── .is_acyclic()        → bool
        └── <Ext methods ...>    (zero-cost, mixed in at compile time)
```

## Build instructions

Requires a **C++23** compliant compiler (GCC 14 / Clang 17 or newer) and **CMake 3.31+**.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

**Testing** (GoogleTest is fetched automatically):
```bash
cd build && ctest --output-on-failure
```

## Quick start

### Basic usage

```cpp
#include <iostream>
#include <dagpp.h>

// Node data must satisfy std::semiregular (default constructible, copyable)
struct Task {
    int id;
    std::string name;
};

int main() {
    dagpp::digraph_builder<Task> builder;

    const auto compile  = builder.add_node({0, "compile"});
    const auto link     = builder.add_node({1, "link"});
    const auto test     = builder.add_node({2, "test"});

    builder.add_edge(compile, link);
    builder.add_edge(link, test);
    
    const auto graph = builder.compile();

    if (auto edges = graph.out_edges(compile); edges) {
        std::cout << "'compile' has " << edges->size() << " outbound edge(s).\n";
    }

    std::cout << "Node 0: " << graph.node(0).name << "\n";

    return 0;
}
```

### In-edges & Cycle detection

The reverse adjacency list is built alongside the forward one during `compile()`.

```cpp
#include <dagpp.h>

struct Node { int value; };

int main() {
    dagpp::digraph_builder<Node> builder;

    auto a = builder.add_node({1});
    auto b = builder.add_node({2});
    auto c = builder.add_node({3});

    builder.add_edge(a, c);
    builder.add_edge(b, c);   // two nodes point to c

    const auto graph = builder.compile();

    // Inbound neighbours of c
    if (auto in = graph.in_edges(c); in) {
        std::cout << "Node c has " << in->size() << " inbound edge(s).\n";
    }

    // Acyclicity check - O(V + E)
    std::cout << (graph.is_acyclic() ? "DAG" : "Cycle detected") << "\n";

    return 0;
}
```

### Topological sort

`topo_sort` is a free function in `<topo_sort.h>` (included via `<dagpp.h>`).

```cpp
#include <iostream>
#include <dagpp.h>

struct Node { int id; };

int main() {
    dagpp::digraph_builder<Node> builder;

    auto a = builder.add_node({0});
    auto b = builder.add_node({1});
    auto c = builder.add_node({2});

    builder.add_edge(a, b);
    builder.add_edge(b, c);

    const auto graph = builder.compile();

    if (auto order = dagpp::topo_sort(graph); order) {
        std::cout << "Topological order: ";
        for (auto id : *order) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    } else {
        std::cerr << "Error: " << order.error() << "\n";
    }

    return 0;
}
```

### DOT export (built-in extension)

Include `<dot.h>` and mix in `dagpp::ext::dot_exporter` at compile time.
Edge direction can be reversed by passing `dagpp::inbound{}` as the direction policy.

```cpp
#include <fstream>
#include <dagpp.h>
#include <dot.h>

struct Node { int id; };

int main() {
    dagpp::digraph_builder<Node> builder;

    auto a = builder.add_node({10});
    auto b = builder.add_node({20});
    auto c = builder.add_node({30});

    builder.add_edge(a, b);
    builder.add_edge(a, c);
    builder.add_edge(b, c);

    const auto graph = builder.compile<dagpp::ext::dot_exporter>();

    std::ofstream out("graph.dot");
    graph.to_dot([](std::size_t i, const Node& n) {
        return std::format("\tn{} [label=\"{}\"];\n", i, n.id);
    }, out);

    // Export reversed (inbound direction)
    // graph.to_dot<dagpp::inbound>([](std::size_t i, const Node& n) { ... }, out);

    return 0;
}
```

## Extensions

An extension is a struct whose methods use C++23 "Deducing This" to receive the concrete `digraph` type as `self`.

```cpp
template<typename TSelf>
void my_method(this const TSelf& self) {
    // self is the fully-typed digraph - access .node(), .count(), .out_edges(), etc.
}
```

### JSON exporter custom extension

```cpp
#include <fstream>
#include <dagpp.h>

struct Node { int id; };

struct json_exporter {
    template<typename TSelf>
    void export_json(this const TSelf& self, std::ofstream& out) {
        out << "{\n  \"nodes\": [\n";
        for (std::size_t i = 0; i < self.count(); ++i) {
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
    dagpp::digraph_builder<Node> builder;

    builder.add_edge(builder.add_node({10}), builder.add_node({20}));

    const auto graph = builder.compile<json_exporter>();

    std::ofstream out("graph.json");
    graph.export_json(out);

    return 0;
}
```

### Multiple extensions

Provide as many extensions as you like - they are all mixed in simultaneously:

```cpp
const auto graph = builder.compile<json_exporter, dagpp::ext::dot_exporter>();

graph.export_json(json_out);
graph.to_dot([](std::size_t i, const Node& n) { ... }, dot_out);
```

---

## API reference

### `digraph_builder<TNode>`

| Method                                      | Description                                                                    |
|---------------------------------------------|--------------------------------------------------------------------------------|
| `nodeid_t add_node(TNode)`                  | Appends a node and returns its stable id                                       |
| `void add_edge(nodeid_t from, nodeid_t to)` | Records a directed edge                                                        |
| `void reserve_nodes(size_t n)`              | Pre-allocates node storage                                                     |
| `void reserve_edges(size_t n)`              | Pre-allocates edge storage                                                     |
| `digraph<TNode, Ext...> compile<Ext...>()`  | Builds the immutable CSR graph; `builder` should not be reused after this call |

> **Note:** `TNode` must satisfy `std::semiregular`. SEE: https://en.cppreference.com/w/cpp/concepts/semiregular

### `digraph<TNode, TExtension...>`

| Method                   | Return Type                                             | Description                                           |
|--------------------------|---------------------------------------------------------|-------------------------------------------------------|
| `node(nodeid_t id)`      | `const TNode&`                                          | Returns the node data at `id`                         |
| `count()`                | `std::size_t`                                           | Total number of nodes                                 |
| `out_edges(nodeid_t id)` | `std::expected<std::span<const nodeid_t>, std::string>` | Outbound neighbour ids; error if `id` is out of range |
| `in_edges(nodeid_t id)`  | `std::expected<std::span<const nodeid_t>, std::string>` | Inbound neighbour ids; error if `id` is out of range  |
| `is_acyclic()`           | `bool`                                                  | `true` if the graph is a DAG; O(V + E)                |

Edge spans (`std::span<const nodeid_t>`) are zero-copy views into the internal CSR arrays.

### `topo_sort`

```cpp
template<node TNode, typename... TExtension>
std::expected<std::vector<nodeid_t>, std::string>
    dagpp::topo_sort(const digraph<TNode, TExtension...>& graph);
```

Kahn's BFS-based topological sort. Returns the sorted node id sequence, or `std::unexpected{"Graph contains a cycle."}` if a cycle is detected.

---

### `dagpp::ext::dot_exporter`

```cpp
template<typename TDir = dagpp::outbound, typename TSelf, typename Pred>
void to_dot(this const TSelf& self,
            const Pred& label_pred,
            std::ofstream& out,
            TDir cmp = TDir{});
```

| Parameter    | Description                                                                       |
|--------------|-----------------------------------------------------------------------------------|
| `cmp`        | Direction policy - `dagpp::outbound` (default) or `dagpp::inbound`                |
| `label_pred` | Callable `(std::size_t index, const node_t& node) -> std::string` for node labels |
| `out`        | Output file stream                                                                |

---

## CMake options

| Option                    | Default | Description                                  |
|---------------------------|---------|----------------------------------------------|
| `DAGPP_BUILD_NATIVE`      | `ON`    | Compile with `-march=native` in Release mode |
| `DAGPP_ENABLE_LTO`        | `ON`    | Enable Link-Time Optimisation                |
| `DAGPP_STRIP_DEAD`        | `ON`    | Strip dead code with `--gc-sections` (Linux) |
| `DAGPP_ENABLE_NO_EXCEPT`  | `ON`    | Add `-fno-exceptions` in Release mode        |
| `DAGPP_ENABLE_NO_RTTI`    | `ON`    | Add `-fno-rtti` in Release mode              |
| `DAGPP_HIDDEN_VISIBILITY` | `ON`    | Hide symbols by default                      |
| `DAGPP_ENABLE_AVX10`      | `ON`    | Use AVX10.1 when not building native         |
| `DAGPP_ENABLE_AVX2`       | `ON`    | Fall back to AVX2 when not building native   |
| `DAGPP_ENABLE_FAST_MATH`  | `OFF`   | Enable `-ffast-math` (not recommended)       |

## License

[MIT](LICENSE)
