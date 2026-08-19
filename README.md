# dag++

A simple and fast C++23 directed graph library.

![C++23](https://img.shields.io/badge/C%2B%2B-23-blue?logo=c%2B%2B)
![CMake](https://img.shields.io/badge/CMake-3.20%2B-064F8C?logo=cmake)
![License: MIT](https://img.shields.io/badge/License-MIT-green)
![CI](https://github.com/inquaterni/dagpp/actions/workflows/cmake-multi-platform.yml/badge.svg)
[![codecov](https://codecov.io/github/inquaterni/dagpp/graph/badge.svg?token=R85MIRY2FG)](https://codecov.io/github/inquaterni/dagpp)

## Table of Contents

- [Features](#features)
- [Build instructions](#build-instructions)
- [Quick start](#quick-start)
  - [Basic usage](#basic-usage)
  - [Weighted mutable graph](#weighted-mutable-graph)
  - [In-edges & Cycle detection](#in-edges--cycle-detection)
  - [Topological sort](#topological-sort)
  - [DOT export (built-in extension)](#dot-export-built-in-extension)
- [Extensions](#extensions)
  - [JSON exporter custom extension](#json-exporter-custom-extension)
  - [Multiple extensions](#multiple-extensions)
- [API reference](#api-reference)
  - [digraph\<TNode, TExtension...\>](#digraphtnode-textension)
  - [wdigraph\<TNode, TWeight, TExtension...\>](#wdigraphtnode-tweight-textension)
  - [csr::digraph_builder\<TNode\>](#csrdigraph_buildertnode)
  - [csr::digraph\<TNode, TExtension...\>](#csrdigraphtnode-textension)
  - [csr::wdigraph\<TNode, TWeight, TExtension...\>](#csrwdigraphtnode-tweight-textension)
  - [vcsr::digraph_builder\<TNode\>](#vcsrdigraph_buildertnode)
  - [vcsr::digraph\<TNode, TDensityPolicy, TExtension...\>](#vcsrdigraphtnode-tdensitypolicy-textension)
  - [vcsr::wdigraph\<TNode, TWeight, TDensityPolicy, TExtension...\>](#vcsrwdigraphtnode-tweight-tdensitypolicy-textension)
  - [topo_sort](#topo_sort)
  - [dijkstra](#dijkstra)
  - [dagpp::ext::dot_exporter](#dagppextdot_exporter)
- [CMake options](#cmake-options)
- [Benchmarks](#benchmarks)
- [License](#license)

---

## Features

- **Mutable representation**: `dagpp::digraph` backed by internal vectors
- **Mutable weighted option**: `dagpp::wdigraph` — mutable weighted graph satisfying the `wdirected_graph` concept
- **Immutable CSR option**: Highly compact `dagpp::csr::digraph` built via `dagpp::csr::digraph_builder`
- **Weighted CSR option**: `dagpp::csr::wdigraph` built via `dagpp::csr::wdigraph_builder` for algorithms like Dijkstra
- Both representations guarantee `out_edges()` / `in_edges()` return `std::span` views with zero extra allocation
- Compile-time extensions via C++23 "Deducing This"
- Topological sort (Kahn's), `is_acyclic()`, and **Dijkstra's Pathfinding** included
- `std::expected` for errors; `-fno-exceptions` / `-fno-rtti` compatible
- Header-only

All implementations satisfy the `dagpp::directed_graph` concept:
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

template <typename T>
concept wdirected_graph = directed_graph<T> && requires (const T &t, dagpp::nodeid_t id)
{
    typename T::weight_type;
    requires std::floating_point<typename T::weight_type> || std::integral<typename T::weight_type>;
    {*t.out_weights(id)} -> std::convertible_to<std::span<const typename T::weight_type>>;
    {*t.in_weights(id)}  -> std::convertible_to<std::span<const typename T::weight_type>>;
};
```

## Build instructions

Requires a C++23 compliant compiler and CMake 3.20+.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
> NOTE: See all CMake options [here](CMakeLists.txt) or (less reliable) [here](#cmake-options)

Testing (GoogleTest is fetched automatically):
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

#### Weighted mutable graph

The mutable `wdigraph` adds per-edge weights and satisfies the `wdirected_graph` concept directly — no build step required.

```cpp
#include <iostream>
#include <dagpp.h>

struct Node { int id; };

int main() {
    dagpp::wdigraph<Node, int> graph;

    const auto a = graph.add_node({0});
    const auto b = graph.add_node({1});
    const auto c = graph.add_node({2});

    graph.add_edge(a, b, 4);
    graph.add_edge(a, c, 1);
    graph.add_edge(c, b, 2);

    // Run Dijkstra directly on the mutable graph
    const auto result = dagpp::dijkstra(graph, a);
    std::cout << "Shortest distance a->b: " << result.distances[b] << "\n"; // 3

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

    std::cout << "CSR graph has " << graph.node_count() << " nodes.\n";

    return 0;
}
```

#### VCSR graph
Similar to CSR, but supports node and edge insertion after compilation via a PMA-backed storage.
```cpp
#include <iostream>
#include <dagpp.h>

// Node data must satisfy std::semiregular ONLY for the VCSR graph
struct Node { 
    int id;
    std::string name;
};

int main() {
    dagpp::vcsr::digraph_builder<Node> builder;

    // Node accumulation is mutable...
    auto a = builder.add_node({0});
    auto b = builder.add_node({1});
    auto c = builder.add_node({2});

    builder.add_edge(a, b);
    builder.add_edge(b, c);

    auto graph = builder.compile();
    // When using VCSR graphs, in order to mix in extensions, you need to pass them to the builder's `compile()` method
    // `dagpp::vcsr::default_thresholds` is a default density thresholds policy, following the constraints of the
    // concept `dagpp::vcsr::density_policy`, which comes first before extension types, and is a basic C-style struct.
    // auto graph_with_ext = builder.compile<dagpp::vcsr::default_thresholds, dagpp::ext::dot_exporter>();
    
    // ... even after graph compilation
    graph.add_node({3, "Bob"});

    std::cout << "VCSR graph has " << graph.node_count() << " nodes.\n";

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
const auto graph = builder.compile<json_exporter, dagpp::ext::dot_exporter>();

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

> NOTE: For the CSR graph, `TNode` must satisfy `std::semiregular`. SEE: https://en.cppreference.com/w/cpp/concepts/semiregular

---

### `wdigraph<TNode, TWeight, TExtension...>`

Mutable weighted digraph. Extends `digraph` with per-edge weights on both forward and reverse adjacency lists.

| Method                                                 | Return Type                                             | Description                                           |
|--------------------------------------------------------|---------------------------------------------------------|-------------------------------------------------------|
| `add_node(TNode)`                                      | `nodeid_t`                                              | Appends a node and returns its stable id              |
| `add_edge(nodeid_t from, nodeid_t to, TWeight weight)` | `void`                                                  | Records a weighted directed edge                      |
| `reserve_nodes(size_t n)`                              | `void`                                                  | Pre-allocates node and adjacency storage              |
| `node(nodeid_t id)`                                    | `const TNode&` or `TNode&`                              | Returns the node data at `id`                         |
| `count()`                                              | `std::size_t`                                           | Total number of nodes                                 |
| `out_edges(nodeid_t id)`                               | `std::expected<std::span<const nodeid_t>, std::string>` | Outbound neighbour ids; error if `id` is out of range |
| `in_edges(nodeid_t id)`                                | `std::expected<std::span<const nodeid_t>, std::string>` | Inbound neighbour ids; error if `id` is out of range  |
| `out_weights(nodeid_t id)`                             | `std::expected<std::span<const TWeight>, std::string>`  | Outbound weights; error if `id` is out of range       |
| `in_weights(nodeid_t id)`                              | `std::expected<std::span<const TWeight>, std::string>`  | Inbound weights; error if `id` is out of range        |
| `is_acyclic()`                                         | `bool`                                                  | `true` if the graph is a DAG; O(V + E)                |

> NOTE: `TWeight` must satisfy the `number` concept (`std::integral` or `std::floating_point`).

---

### `csr::digraph_builder<TNode>`

| Method                                  | Return Type                     | Description                                                                    |
|-----------------------------------------|---------------------------------|--------------------------------------------------------------------------------|
| `add_node(TNode)`                       | `nodeid_t`                      | Appends a node and returns its stable id                                       |
| `add_edge(nodeid_t from, nodeid_t to)`  | `void`                          | Records a directed edge                                                        |
| `reserve_nodes(size_t n)`               | `void`                          | Pre-allocates node storage                                                     |
| `reserve_edges(size_t n)`               | `void`                          | Pre-allocates edge storage                                                     |
| `compile<Ext...>()`                     | `csr::digraph<TNode, Ext...>`   | Builds the immutable CSR graph; `builder` should not be reused after this call |

### `csr::digraph<TNode, TExtension...>`

| Method                   | Return Type                                             | Description                                           |
|--------------------------|---------------------------------------------------------|-------------------------------------------------------|
| `node(nodeid_t id)`      | `const TNode&`                                          | Returns the node data at `id`                         |
| `count()`                | `std::size_t`                                           | Total number of nodes                                 |
| `out_edges(nodeid_t id)` | `std::expected<std::span<const nodeid_t>, std::string>` | Outbound neighbour ids; error if `id` is out of range |
| `in_edges(nodeid_t id)`  | `std::expected<std::span<const nodeid_t>, std::string>` | Inbound neighbour ids; error if `id` is out of range  |
| `is_acyclic()`           | `bool`                                                  | `true` if the graph is a DAG; O(V + E)                |

Edge spans are non-owning views into the internal CSR arrays.

### `csr::wdigraph<TNode, TWeight, TExtension...>`

Similar to `csr::digraph`, but additionally provides:

| Method                      | Return Type                                               | Description                                        |
|-----------------------------|-----------------------------------------------------------|----------------------------------------------------|
| `out_weights(nodeid_t id)`  | `std::expected<std::span<const TWeight>, std::string>`    | Outbound weights; error if `id` is out of range    |
| `in_weights(nodeid_t id)`   | `std::expected<std::span<const TWeight>, std::string>`    | Inbound weights; error if `id` is out of range     |

And `wdigraph_builder::add_edge` takes a third parameter for the `weight`.

---

### `vcsr::digraph_builder<TNode>`

| Method                                  | Return Type                                       | Description                                                                    |
|-----------------------------------------|---------------------------------------------------|--------------------------------------------------------------------------------|
| `add_node(TNode)`                       | `nodeid_t`                                        | Appends a node and returns its stable id                                       |
| `add_edge(nodeid_t from, nodeid_t to)`  | `void`                                            | Records a directed edge                                                        |
| `reserve_nodes(size_t n)`               | `void`                                            | Pre-allocates node storage                                                     |
| `reserve_edges(size_t n)`               | `void`                                            | Pre-allocates edge storage                                                     |
| `compile<DensityPolicy, Ext...>()`      | `vcsr::digraph<TNode, DensityPolicy, Ext...>`     | Builds the PMA-backed mutable CSR graph                                        |

### `vcsr::digraph<TNode, TDensityPolicy, TExtension...>`

A mutable CSR storage format that leverages the Packed Memory Array (PMA) via a new vertex-centric strategy to store temporal graphs efficiently.
Supports dynamic `add_node()` and `add_edge()` after compilation, with automatic implicit tree rebalancing. [Reference Implementation](https://github.com/DIR-LAB/VCSR)

| Method                                      | Return Type                                             | Description                                                |
|---------------------------------------------|---------------------------------------------------------|------------------------------------------------------------|
| `add_node(TNode)`                           | `nodeid_t`                                              | Dynamically adds a node (triggers PMA expansion if needed) |
| `add_edge(nodeid_t from, nodeid_t to)`      | `void`                                                  | Inserts an edge with PMA rebalancing                       |
| `node(nodeid_t id)`                         | `const TNode&`                                          | Returns the node data at `id`                              |
| `node_count()`                              | `std::size_t`                                           | Total number of nodes                                      |
| `edge_count()`                              | `std::size_t`                                           | Total number of edges                                      |
| `out_edges(nodeid_t id)`                    | `std::expected<std::span<const nodeid_t>, std::string>` | Outbound neighbour ids; error if `id` is out of range      |
| `in_edges(nodeid_t id)`                     | `std::expected<std::span<const nodeid_t>, std::string>` | Inbound neighbour ids; error if `id` is out of range       |
| `is_acyclic()`                              | `bool`                                                  | `true` if the graph is a DAG; O(V + E)                     |

### `vcsr::wdigraph<TNode, TWeight, TDensityPolicy, TExtension...>`

Weighted variant. Extends `vcsr::digraph` with per-edge weights.

| Method                                                 | Return Type                                              | Description                                           |
|--------------------------------------------------------|----------------------------------------------------------|-------------------------------------------------------|
| `add_node(TNode)`                                      | `nodeid_t`                                               | Dynamically adds a node                               |
| `add_edge(nodeid_t from, nodeid_t to, TWeight weight)` | `void`                                                   | Inserts a weighted edge with PMA rebalancing          |
| `out_weights(nodeid_t id)`                             | `std::expected<std::span<const TWeight>, std::string>`   | Outbound weights; error if `id` is out of range       |
| `in_weights(nodeid_t id)`                              | `std::expected<std::span<const TWeight>, std::string>`   | Inbound weights; error if `id` is out of range        |

Uses `std::pmr::polymorphic_allocator` for all internal storage, supporting custom memory resources.

---

### `topo_sort`

```cpp
template<directed_graph TGraph>
constexpr std::expected<std::vector<nodeid_t>, std::string>
    dagpp::topo_sort(const TGraph& graph);
```

Kahn's BFS-based topological sort. Returns the sorted node id sequence, or `std::unexpected` if a cycle is detected.

---

### `dijkstra`

```cpp
template <wdirected_graph TGraph>
constexpr dijkstra_result<typename TGraph::weight_type>
    dagpp::dijkstra(const TGraph& g, nodeid_t source);
```

Computes shortest paths using Dijkstra's algorithm. Returns a result struct containing `.distances` and `.previous` path vectors. Fully `constexpr` compatible.

---

### `dagpp::ext::dot_exporter`

```cpp
template<typename TDir = dagpp::outbound, directed_graph TSelf, typename Pred>
requires std::invocable<Pred, std::size_t, typename TSelf::node_type> &&
         std::convertible_to<std::invoke_result_t<Pred, typename TSelf::size_type, typename TSelf::node_type>, std::string>
constexpr void to_dot(this const TSelf& self,
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
| `DAGPP_ENABLE_AVX10`      | `OFF`   | Use AVX10.1 when not building native         |
| `DAGPP_ENABLE_AVX2`       | `OFF`   | Fall back to AVX2 when not building native   |
| `DAGPP_ENABLE_FAST_MATH`  | `OFF`   | Enable `-ffast-math` (not recommended)       |

## Benchmarks

To compile and run the benchmarks locally:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target dagpp_bench -j$(nproc)
./build/bench/dagpp_bench
```

Performance is measured using `csr::digraph`/`csr::wdigraph` across different graph topologies and sizes.
VCSR benchmarks are not included here due to higher memory consumption and resulting benchmark instability.

| Benchmark                                    | Time      | CPU Time  | Iterations | Throughput |
|----------------------------------------------|-----------|-----------|------------|------------|
| **`BM_ConstructChain/64`**                   | 9.97 μs   | 9.93 μs   | 58776      | 6.447M/s   |
| **`BM_ConstructChain/256`**                  | 33.68 μs  | 33.56 μs  | 20829      | 7.62804M/s |
| **`BM_ConstructChain/1024`**                 | 133.15 μs | 132.66 μs | 5296       | 7.71898M/s |
| **`BM_ConstructChain/4096`**                 | 674.23 μs | 671.20 μs | 1043       | 6.10249M/s |
| **`BM_ConstructChain/16384`**                | 3.28 ms   | 3.26 ms   | 243        | 5.03101M/s |
| **`BM_ConstructChain/65536`**                | 11.43 ms  | 11.37 ms  | 60         | 5.76508M/s |
| **`BM_ConstructBinaryTree/64`**              | 9.58 μs   | 9.55 μs   | 73851      | 6.69792M/s |
| **`BM_ConstructBinaryTree/256`**             | 37.12 μs  | 37.00 μs  | 18897      | 6.91849M/s |
| **`BM_ConstructBinaryTree/1024`**            | 143.25 μs | 142.79 μs | 4880       | 7.17112M/s |
| **`BM_ConstructBinaryTree/4096`**            | 608.78 μs | 606.45 μs | 1296       | 6.75405M/s |
| **`BM_ConstructBinaryTree/16384`**           | 3.06 ms   | 3.04 ms   | 230        | 5.38219M/s |
| **`BM_ConstructBinaryTree/65536`**           | 17.40 ms  | 17.30 ms  | 41         | 3.78816M/s |
| **`BM_ConstructDenseDag/64`**                | 20.19 μs  | 20.11 μs  | 34653      | 100.253M/s |
| **`BM_ConstructDenseDag/256`**               | 160.91 μs | 160.13 μs | 4355       | 203.837M/s |
| **`BM_ConstructDenseDag/1024`**              | 5.98 ms   | 5.94 ms   | 119        | 88.2425M/s |
| **`BM_ConstructDenseDag/4096`**              | 79.77 ms  | 79.03 ms  | 8          | 106.114M/s |
| **`BM_ConstructDenseDag/8192`**              | 311.62 ms | 308.68 ms | 2          | 108.688M/s |
| **`BM_OutEdgesChain/64`**                    | 105 ns    | 104 ns    | 6697956    | 613.986M/s |
| **`BM_OutEdgesChain/256`**                   | 396 ns    | 394 ns    | 1653974    | 649.594M/s |
| **`BM_OutEdgesChain/1024`**                  | 1.21 μs   | 1.21 μs   | 581364     | 846.049M/s |
| **`BM_OutEdgesChain/4096`**                  | 4.82 μs   | 4.80 μs   | 145217     | 853.057M/s |
| **`BM_OutEdgesChain/16384`**                 | 19.38 μs  | 19.31 μs  | 36314      | 848.331M/s |
| **`BM_OutEdgesChain/65536`**                 | 78.22 μs  | 77.90 μs  | 8979       | 841.239M/s |
| **`BM_InEdgesChain/64`**                     | 99 ns     | 99 ns     | 9302806    | 647.063M/s |
| **`BM_InEdgesChain/256`**                    | 424 ns    | 422 ns    | 1656346    | 606.48M/s  |
| **`BM_InEdgesChain/1024`**                   | 1.67 μs   | 1.66 μs   | 419912     | 616.139M/s |
| **`BM_InEdgesChain/4096`**                   | 6.67 μs   | 6.65 μs   | 105426     | 616.064M/s |
| **`BM_InEdgesChain/16384`**                  | 21.30 μs  | 21.23 μs  | 26327      | 771.876M/s |
| **`BM_InEdgesChain/65536`**                  | 77.72 μs  | 77.43 μs  | 9051       | 846.359M/s |
| **`BM_OutEdgesDense/64`**                    | 556 ns    | 555 ns    | 1260863    | 3.63355G/s |
| **`BM_OutEdgesDense/256`**                   | 9.75 μs   | 9.72 μs   | 71933      | 3.35775G/s |
| **`BM_OutEdgesDense/1024`**                  | 148.42 μs | 147.84 μs | 4739       | 3.54285G/s |
| **`BM_OutEdgesDense/4096`**                  | 3.40 ms   | 3.35 ms   | 279        | 2.50056G/s |
| **`BM_OutEdgesDense/8192`**                  | 14.08 ms  | 13.91 ms  | 52         | 2.41245G/s |
| **`BM_IsAcyclicTrue/64`**                    | 287 ns    | 286 ns    | 2442097    | 223.635M/s |
| **`BM_IsAcyclicTrue/256`**                   | 939 ns    | 935 ns    | 599438     | 273.662M/s |
| **`BM_IsAcyclicTrue/1024`**                  | 3.68 μs   | 3.67 μs   | 190998     | 279.262M/s |
| **`BM_IsAcyclicTrue/4096`**                  | 13.91 μs  | 13.88 μs  | 50145      | 295.161M/s |
| **`BM_IsAcyclicTrue/16384`**                 | 53.30 μs  | 53.16 μs  | 13052      | 308.228M/s |
| **`BM_IsAcyclicTrue/65536`**                 | 217.40 μs | 216.61 μs | 3243       | 302.547M/s |
| **`BM_IsAcyclicFalse/64`**                   | 62 ns     | 62 ns     | 10000000   | 1.0295G/s  |
| **`BM_IsAcyclicFalse/256`**                  | 230 ns    | 229 ns    | 3059389    | 1.11905G/s |
| **`BM_IsAcyclicFalse/1024`**                 | 861 ns    | 858 ns    | 817894     | 1.19412G/s |
| **`BM_IsAcyclicFalse/4096`**                 | 3.01 μs   | 3.00 μs   | 232004     | 1.36434G/s |
| **`BM_IsAcyclicFalse/16384`**                | 11.58 μs  | 11.54 μs  | 58924      | 1.42039G/s |
| **`BM_IsAcyclicFalse/65536`**                | 33.76 μs  | 33.65 μs  | 20796      | 1.94759G/s |
| **`BM_IsAcyclicDense/64`**                   | 877 ns    | 875 ns    | 809151     | 73.1395M/s |
| **`BM_IsAcyclicDense/256`**                  | 12.22 μs  | 12.19 μs  | 57497      | 21.0038M/s |
| **`BM_IsAcyclicDense/1024`**                 | 184.77 μs | 184.05 μs | 3749       | 5.5636M/s  |
| **`BM_IsAcyclicDense/4096`**                 | 5.08 ms   | 5.02 ms   | 153        | 815.277k/s |
| **`BM_IsAcyclicDense/8192`**                 | 19.09 ms  | 18.88 ms  | 36         | 433.964k/s |
| **`BM_TopoSortChain/64`**                    | 480 ns    | 479 ns    | 1435706    | 133.729M/s |
| **`BM_TopoSortChain/256`**                   | 1.37 μs   | 1.36 μs   | 514065     | 187.914M/s |
| **`BM_TopoSortChain/1024`**                  | 6.10 μs   | 6.08 μs   | 115135     | 168.384M/s |
| **`BM_TopoSortChain/4096`**                  | 22.25 μs  | 22.19 μs  | 31614      | 184.615M/s |
| **`BM_TopoSortChain/16384`**                 | 95.36 μs  | 95.07 μs  | 7363       | 172.34M/s  |
| **`BM_TopoSortChain/65536`**                 | 473.29 μs | 471.06 μs | 1865       | 139.124M/s |
| **`BM_TopoSortBinaryTree/64`**               | 385 ns    | 384 ns    | 1829334    | 166.684M/s |
| **`BM_TopoSortBinaryTree/256`**              | 1.53 μs   | 1.53 μs   | 453880     | 167.55M/s  |
| **`BM_TopoSortBinaryTree/1024`**             | 6.51 μs   | 6.48 μs   | 109068     | 157.94M/s  |
| **`BM_TopoSortBinaryTree/4096`**             | 25.89 μs  | 25.78 μs  | 27157      | 158.889M/s |
| **`BM_TopoSortBinaryTree/16384`**            | 102.71 μs | 102.27 μs | 6788       | 160.202M/s |
| **`BM_TopoSortBinaryTree/65536`**            | 411.76 μs | 409.72 μs | 1667       | 159.953M/s |
| **`BM_TopoSortDense/64`**                    | 3.11 μs   | 3.10 μs   | 227199     | 20.6763M/s |
| **`BM_TopoSortDense/256`**                   | 39.12 μs  | 38.98 μs  | 14963      | 6.56759M/s |
| **`BM_TopoSortDense/1024`**                  | 501.62 μs | 499.58 μs | 1394       | 2.04972M/s |
| **`BM_TopoSortDense/4096`**                  | 10.17 ms  | 10.08 ms  | 69         | 406.311k/s |
| **`BM_TopoSortDense/8192`**                  | 39.77 ms  | 39.38 ms  | 17         | 208.02k/s  |
| **`BM_NodeAccessSequential/64`**             | 22 ns     | 21 ns     | 47790416   | 2.98542G/s |
| **`BM_NodeAccessSequential/256`**            | 83 ns     | 82 ns     | 8522626    | 3.10932G/s |
| **`BM_NodeAccessSequential/1024`**           | 650 ns    | 648 ns    | 1081003    | 1.58099G/s |
| **`BM_NodeAccessSequential/4096`**           | 2.60 μs   | 2.59 μs   | 270238     | 1.58083G/s |
| **`BM_NodeAccessSequential/16384`**          | 11.69 μs  | 11.64 μs  | 52245      | 1.4071G/s  |
| **`BM_NodeAccessSequential/65536`**          | 36.09 μs  | 35.95 μs  | 19656      | 1.82314G/s |
| **`BM_NodeAccessRandom/64`**                 | 21 ns     | 21 ns     | 32895950   | 3.0169G/s  |
| **`BM_NodeAccessRandom/256`**                | 85 ns     | 85 ns     | 8047732    | 3.01254G/s |
| **`BM_NodeAccessRandom/1024`**               | 528 ns    | 527 ns    | 1317737    | 1.94397G/s |
| **`BM_NodeAccessRandom/4096`**               | 2.92 μs   | 2.91 μs   | 331364     | 1.40587G/s |
| **`BM_NodeAccessRandom/16384`**              | 20.06 μs  | 19.96 μs  | 35996      | 820.85M/s  |
| **`BM_NodeAccessRandom/65536`**              | 103.75 μs | 103.16 μs | 6875       | 635.297M/s |
| **`BM_DijkstraChain/64`**                    | 706 ns    | 704 ns    | 993268     | 90.9687M/s |
| **`BM_DijkstraChain/256`**                   | 2.47 μs   | 2.46 μs   | 235163     | 104M/s     |
| **`BM_DijkstraChain/1024`**                  | 7.73 μs   | 7.71 μs   | 88572      | 132.78M/s  |
| **`BM_DijkstraChain/4096`**                  | 33.46 μs  | 33.36 μs  | 21528      | 122.772M/s |
| **`BM_DijkstraChain/16384`**                 | 131.70 μs | 131.30 μs | 5334       | 124.784M/s |
| **`BM_DijkstraChain/65536`**                 | 528.34 μs | 526.30 μs | 1329       | 124.522M/s |
| **`BM_DijkstraBinaryTree/64`**               | 897 ns    | 894 ns    | 951813     | 71.585M/s  |
| **`BM_DijkstraBinaryTree/256`**              | 5.16 μs   | 5.14 μs   | 136206     | 49.8272M/s |
| **`BM_DijkstraBinaryTree/1024`**             | 25.33 μs  | 25.23 μs  | 27544      | 40.5822M/s |
| **`BM_DijkstraBinaryTree/4096`**             | 127.43 μs | 126.95 μs | 5520       | 32.2647M/s |
| **`BM_DijkstraBinaryTree/16384`**            | 951.02 μs | 946.48 μs | 707        | 17.3105M/s |
| **`BM_DijkstraBinaryTree/65536`**            | 3.92 ms   | 3.91 ms   | 139        | 16.7793M/s |
| **`BM_DijkstraDense/64`**                    | 3.48 μs   | 3.47 μs   | 201932     | 18.4291M/s |
| **`BM_DijkstraDense/128`**                   | 11.50 μs  | 11.47 μs  | 60812      | 11.1614M/s |
| **`BM_DijkstraDense/256`**                   | 37.40 μs  | 37.29 μs  | 18797      | 6.86583M/s |
| **`BM_DijkstraDense/512`**                   | 142.35 μs | 141.77 μs | 5193       | 3.61146M/s |
| **`BM_DijkstraDense/1024`**                  | 695.65 μs | 691.93 μs | 1002       | 1.47993M/s |
| **`BM_DijkstraDense/2048`**                  | 3.34 ms   | 3.31 ms   | 218        | 617.82k/s  |
| **`BM_DijkstraDense/4096`**                  | 13.12 ms  | 12.98 ms  | 54         | 315.527k/s |
| **`BM_DijkstraDense/8192`**                  | 33.27 ms  | 32.92 ms  | 20         | 248.871k/s |
| **`BM_DijkstraSparse/1024`**                 | 24.26 μs  | 24.19 μs  | 28860      | 42.3297M/s |
| **`BM_DijkstraSparse/4096`**                 | 102.82 μs | 102.51 μs | 6888       | 39.9563M/s |
| **`BM_DijkstraSparse/16384`**                | 1.16 ms   | 1.16 ms   | 848        | 14.1715M/s |
| **`BM_DijkstraSparse/65536`**                | 5.03 ms   | 5.00 ms   | 120        | 13.0978M/s |
| **`BM_DijkstraSparse_PointToPoint/256`**     | 735 ns    | 732 ns    | 956217     | 349.627M/s |
| **`BM_DijkstraSparse_PointToPoint/1024`**    | 2.10 μs   | 2.09 μs   | 255004     | 489.381M/s |
| **`BM_DijkstraSparse_PointToPoint/4096`**    | 8.80 μs   | 8.78 μs   | 78710      | 466.621M/s |
| **`BM_DijkstraSparse_PointToPoint/16384`**   | 34.02 μs  | 33.93 μs  | 20687      | 482.917M/s |
| **`BM_DijkstraSparse_PointToPoint/65536`**   | 162.28 μs | 161.65 μs | 3926       | 405.414M/s |
| **`BM_DijkstraSparse_PointToPoint/262144`**  | 1.08 ms   | 1.08 ms   | 648        | 243.348M/s |
| **`BM_DijkstraSparse_PointToPoint/1048576`** | 6.76 ms   | 6.72 ms   | 92         | 156.057M/s |
> NOTE:
  *Hardware context: Ryzen 5 PRO 7540U, 12x 3.16 GHz CPUs, 16 MB L3 cache.
  Built with -O3, -march=native and Google Benchmark.*


## License

[MIT](LICENSE)
