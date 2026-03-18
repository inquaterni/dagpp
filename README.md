# dag++

A simple and fast C++23 directed graph library.

![C++23](https://img.shields.io/badge/C%2B%2B-23-blue?logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.31%2B-064F8C?logo=cmake)
![License: MIT](https://img.shields.io/badge/License-MIT-green)
![CI](https://github.com/inquaterni/dagpp/actions/workflows/cmake-multi-platform.yml/badge.svg)
[![codecov](https://codecov.io/github/inquaterni/dagpp/graph/badge.svg?token=R85MIRY2FG)](https://codecov.io/github/inquaterni/dagpp)
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
  - [digraph\<TNode, TExtension...\>](#digraphtnode-textension)
  - [csr::digraph_builder\<TNode\>](#csrdigraph_buildertnode)
  - [csr::digraph\<TNode, TExtension...\>](#csrdigraphtnode-textension)
  - [topo_sort](#topo_sort)
  - [dagpp::ext::dot_exporter](#dagppextdot_exporter)
- [CMake options](#cmake-options)
- [Benchmarks](#benchmarks)
- [License](#license)

---

## Features

- **Mutable representation**: `dagpp::digraph` backed by internal vectors
- **Immutable CSR option**: Highly compact `dagpp::csr::digraph` built via `dagpp::csr::digraph_builder`
- Both representations guarantee `out_edges()` / `in_edges()` return `std::span` views with zero extra allocation
- Compile-time extensions via C++23 "Deducing This" (no vtables)
- Topological sort (Kahn's) and `is_acyclic()` included
- `std::expected` for errors; `-fno-exceptions` / `-fno-rtti` compatible
- Header-only

## Architecture

```
digraph<TNode, Ext...>          (mutable, dynamic)
        ├── .add_node(TNode)     → nodeid_t
        ├── .add_edge(from, to)
        ├── <Ext methods ...>    (zero-cost mixins)

csr::digraph_builder<TNode>     (mutable, accumulates nodes & edges)
        │
        │  .compile<Ext1, Ext2, ...>()
        ▼
csr::digraph<TNode, Ext1...>    (immutable CSR, inherits from each Ext)

# Both representations satisfy the common graph interface:
        ├── .out_edges(id)       → std::expected<std::span<const nodeid_t>, string>
        ├── .in_edges(id)        → std::expected<std::span<const nodeid_t>, string>
        ├── .node(id)            → const TNode&
        ├── .count()             → std::size_t
        ├── .is_acyclic()        → bool
        └── <Ext methods ...>    (zero-cost mixins)
```

Both implementations satisfy the `dagpp::directed_graph` concept:
```cpp
template <typename T>
concept directed_graph = requires (const T &t, dagpp::nodeid_t id)
{
    typename T::node_type;
    typename T::size_type;
    {*t.out_edges(id)} -> std::convertible_to<std::span<const dagpp::nodeid_t>>;
    {*t.in_edges(id)}  -> std::convertible_to<std::span<const dagpp::nodeid_t>>;
    {t.is_acyclic()}   -> std::same_as<bool>;
    {t.node(id)}       -> std::convertible_to<typename T::node_type>;
    {t.count()}        -> std::same_as<typename T::size_type>;
};
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
#### Generic mutable graph

```cpp
#include <iostream>
#include <dagpp.h>

struct Task {
    int id;
    std::string name;
};

int main() {
    dagpp::digraph<Task> graph;

    const auto compile  = graph.add_node({0, "compile"});
    const auto link     = graph.add_node({1, "link"});
    const auto test     = graph.add_node({2, "test"});

    graph.add_edge(compile, link);
    graph.add_edge(link, test);
    
    if (auto edges = graph.out_edges(compile); edges) {
        std::cout << "'compile' has " << edges->size() << " outbound edge(s).\n";
    }

    std::cout << "Node 0: " << graph.node(0).name << "\n";

    return 0;
}
```

#### Immutable CSR graph

When your graph topology is final, use the highly compact `csr::digraph`:

```cpp
#include <iostream>
#include <dagpp.h>

// Node data must satisfy std::semiregular ONLY for the CSR graph
struct Node { 
    int id;
    std::string name;
};

int main() {
    dagpp::csr::digraph_builder<Node> builder;

    // Node accumulation is mutable...
    auto a = builder.add_node({0});
    auto b = builder.add_node({1});
    auto c = builder.add_node({2});

    builder.add_edge(a, b);
    builder.add_edge(b, c);

    // ...until compilation into an immutable graph
    const auto graph = builder.compile();
    // When using CSR graphs, in order to mix in extensions, you need to pass them to the builder's `compile()` method
    // const auto graph_with_ext = builder.compile<dagpp::ext::dot_exporter>();

    std::cout << "CSR graph has " << graph.count() << " nodes.\n";

    return 0;
}
```

### In-edges & Cycle detection

The reverse adjacency list is built alongside the forward one during `compile()`.

```cpp
#include <dagpp.h>

struct Node { int value; };

int main() {
    dagpp::csr::digraph_builder<Node> builder;

    auto a = builder.add_node({1});
    auto b = builder.add_node({2});
    auto c = builder.add_node({3});

    builder.add_edge(a, c);
    builder.add_edge(b, c);

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
    dagpp::digraph<Node> graph;

    auto a = graph.add_node({0});
    auto b = graph.add_node({1});
    auto c = graph.add_node({2});

    graph.add_edge(a, b);
    graph.add_edge(b, c);

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
#include <format>
#include <dagpp.h>
#include <dot.h>

struct Node { int id; };

int main() {
    dagpp::digraph<Node, dagpp::ext::dot_exporter> graph;

    auto a = graph.add_node({10});
    auto b = graph.add_node({20});
    auto c = graph.add_node({30});

    graph.add_edge(a, b);
    graph.add_edge(a, c);
    graph.add_edge(b, c);

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
    dagpp::digraph<Node, json_exporter> graph;

    const auto a = graph.add_node({10});
    const auto b = graph.add_node({20});
    graph.add_edge(a, b);

    std::ofstream out("graph.json");
    graph.export_json(out);

    return 0;
}
```

### Multiple extensions

Provide as many extensions as you like - they are all mixed in simultaneously:

```cpp
dagpp::digraph<Node, json_exporter, dagpp::ext::dot_exporter> graph;
// OR
const auto csr_graph = builder.compile<json_exporter, dagpp::ext::dot_exporter>();


graph.export_json(json_out);
graph.to_dot([](std::size_t i, const Node& n) { ... }, dot_out);
```

---

## API reference

### `digraph<TNode, TExtension...>`

| Method                                      | Return Type                                             | Description                                           |
|---------------------------------------------|---------------------------------------------------------|-------------------------------------------------------|
| `add_node(TNode)`                           | `nodeid_t`                                              | Appends a node and returns its stable id              |
| `add_edge(nodeid_t from, nodeid_t to)`      | `void`                                                  | Records a directed edge                               |
| `reserve_nodes(size_t n)`                   | `void`                                                  | Pre-allocates node storage                            |
| `node(nodeid_t id)`                         | `const TNode&` or `TNode&`                              | Returns the node data at `id`                         |
| `count()`                                   | `std::size_t`                                           | Total number of nodes                                 |
| `out_edges(nodeid_t id)`                    | `std::expected<std::span<const nodeid_t>, std::string>` | Outbound neighbour ids; error if `id` is out of range |
| `in_edges(nodeid_t id)`                     | `std::expected<std::span<const nodeid_t>, std::string>` | Inbound neighbour ids; error if `id` is out of range  |
| `is_acyclic()`                              | `bool`                                                  | `true` if the graph is a DAG; O(V + E)                |

> **Note:** For the CSR graph, `TNode` must satisfy `std::semiregular`. SEE: https://en.cppreference.com/w/cpp/concepts/semiregular

---

### `csr::digraph_builder<TNode>`

| Method                                          | Description                                                                    |
|-------------------------------------------------|--------------------------------------------------------------------------------|
| `nodeid_t add_node(TNode)`                      | Appends a node and returns its stable id                                       |
| `void add_edge(nodeid_t from, nodeid_t to)`     | Records a directed edge                                                        |
| `void reserve_nodes(size_t n)`                  | Pre-allocates node storage                                                     |
| `void reserve_edges(size_t n)`                  | Pre-allocates edge storage                                                     |
| `csr::digraph<TNode, Ext...> compile<Ext...>()` | Builds the immutable CSR graph; `builder` should not be reused after this call |

### `csr::digraph<TNode, TExtension...>`

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
template<directed_graph TGraph>
constexpr std::expected<std::vector<nodeid_t>, std::string>
    dagpp::topo_sort(const TGraph& graph);
```

Kahn's BFS-based topological sort. Returns the sorted node id sequence, or `std::unexpected{"Graph contains a cycle."}` if a cycle is detected.

---

### `dagpp::ext::dot_exporter`

```cpp
template<typename TDir = dagpp::outbound, directed_graph TSelf, typename Pred>
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

## Benchmarks

> **Note**: Benchmark memory tracking (`GraphMem` / `TopoTempMem`) is Linux/glibc-only because it relies on `mallinfo2()`. On other platforms, memory usage will be reported as 0, but the benchmarks will still compile and run to measure execution time.

To compile and run the benchmarks locally:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target dagpp_bench -j$(nproc)
./build/bench/dagpp_bench
```

Performance is measured on a typical linear chain graph comprising `65,536` nodes with `65,535` edges, using the immutable `csr::digraph`.

| Benchmark               | CPU Time | Throughput      | Memory Footprint (`GraphMem`) | Temporary Mem (`TopoTempMem`) |
|-------------------------|----------|-----------------|-------------------------------|-------------------------------|
| **`BM_ConstructChain`** | 17.09 ms | 3.83 M items/s  | 9.00 MiB                      | -                             |
| **`BM_OutEdgesChain`**  | 0.09 ms  | 685.8 M items/s | 9.00 MiB                      | -                             |
| **`BM_InEdgesChain`**   | 0.08 ms  | 761.7 M items/s | 9.00 MiB                      | -                             |
| **`BM_TopoSortChain`**  | 0.33 ms  | 196.1 M items/s | 9.00 MiB                      | 0 bytes                       |

*Hardware context: Ryzen 5 PRO 7540U, 12x 3.16 GHz CPUs, 16 MB L3 cache.
Built with `-O3` and Google Benchmark.*


## License

[MIT](LICENSE)
