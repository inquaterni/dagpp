/*
* MIT License
 *
 * Copyright (c) 2026 Maksym Matskevich
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <benchmark/benchmark.h>
#include <malloc.h>
#include <numeric>
#include <random>
#include <string>
#include <format>

#include "dagpp.h"

// ---------------------------------------------------------------------------
// Node type
// ---------------------------------------------------------------------------

struct node_t {
    std::string name;
    std::string label;
};

static node_t make_node(std::size_t i) {
    return node_t{
        std::format("task_{:04d}", i),
        std::format("build:lib:task_{:04d}", i)
    };
}

// ---------------------------------------------------------------------------
// Graph factories — unweighted
// ---------------------------------------------------------------------------

static dagpp::csr::digraph<node_t> make_chain(const std::size_t n) {
    dagpp::csr::digraph_builder<node_t> b;
    b.reserve_nodes(n);
    b.reserve_edges(n > 0 ? n - 1 : 0);
    for (std::size_t i = 0; i < n; ++i) b.add_node(make_node(i));
    for (std::size_t i = 0; i + 1 < n; ++i) b.add_edge(i, i + 1);
    return b.compile();
}

static dagpp::csr::digraph<node_t> make_binary_tree(const std::size_t n) {
    dagpp::csr::digraph_builder<node_t> b;
    b.reserve_nodes(n);
    b.reserve_edges(n > 0 ? n - 1 : 0);
    for (std::size_t i = 0; i < n; ++i) b.add_node(make_node(i));
    for (std::size_t i = 0; i < n; ++i) {
        if (const std::size_t l = 2 * i + 1; l < n) b.add_edge(i, l);
        if (const std::size_t r = 2 * i + 2; r < n) b.add_edge(i, r);
    }
    return b.compile();
}

static dagpp::csr::digraph<node_t> make_dense_dag(const std::size_t n) {
    dagpp::csr::digraph_builder<node_t> b;
    b.reserve_nodes(n);
    b.reserve_edges(n * (n - 1) / 2);
    for (std::size_t i = 0; i < n; ++i) b.add_node(make_node(i));
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = i + 1; j < n; ++j)
            b.add_edge(i, j);
    return b.compile();
}

static dagpp::csr::digraph<node_t> make_cyclic(const std::size_t n) {
    dagpp::csr::digraph_builder<node_t> b;
    b.reserve_nodes(n);
    b.reserve_edges(n);
    for (std::size_t i = 0; i < n; ++i) b.add_node(make_node(i));
    for (std::size_t i = 0; i < n; ++i) b.add_edge(i, (i + 1) % n);
    return b.compile();
}

// ---------------------------------------------------------------------------
// Graph factories — weighted
// ---------------------------------------------------------------------------

static dagpp::csr::wdigraph<node_t, std::size_t> make_chain_weighted(std::size_t n) {
    std::mt19937_64 rng{69420};
    std::uniform_int_distribution<std::size_t> w{1, 1000};
    dagpp::csr::wdigraph_builder<node_t, std::size_t> b;
    b.reserve_nodes(n);
    b.reserve_edges(n > 0 ? n - 1 : 0);
    for (std::size_t i = 0; i < n; ++i) b.add_node(make_node(i));
    for (std::size_t i = 0; i + 1 < n; ++i) b.add_edge(i, i + 1, w(rng));
    return b.compile();
}

static dagpp::csr::wdigraph<node_t, std::size_t> make_binary_tree_weighted(const std::size_t n) {
    std::mt19937_64 rng{69420};
    std::uniform_int_distribution<std::size_t> w{1, 1000};
    dagpp::csr::wdigraph_builder<node_t, std::size_t> b;
    b.reserve_nodes(n);
    b.reserve_edges(n > 0 ? n - 1 : 0);
    for (std::size_t i = 0; i < n; ++i) b.add_node(make_node(i));
    for (std::size_t i = 0; i < n; ++i) {
        if (const std::size_t l = 2 * i + 1; l < n) b.add_edge(i, l, w(rng));
        if (const std::size_t r = 2 * i + 2; r < n) b.add_edge(i, r, w(rng));
    }
    return b.compile();
}

static dagpp::csr::wdigraph<node_t, std::size_t> make_dense_dag_weighted(const std::size_t n) {
    std::mt19937_64 rng{69420};
    std::uniform_int_distribution<std::size_t> w{1, 1000};
    dagpp::csr::wdigraph_builder<node_t, std::size_t> b;
    b.reserve_nodes(n);
    b.reserve_edges(n * (n - 1) / 2);
    for (std::size_t i = 0; i < n; ++i) b.add_node(make_node(i));
    for (std::size_t i = 0; i < n; ++i)
        for (std::size_t j = i + 1; j < n; ++j)
            b.add_edge(i, j, w(rng));
    return b.compile();
}

// Sparse random DAG: avg_out forward edges per node via reservoir sampling.
static dagpp::csr::wdigraph<node_t, std::size_t>
make_sparse_dag_weighted(const std::size_t n, const std::size_t avg_out = 12) {
    std::mt19937_64 rng{69420};
    std::uniform_int_distribution<std::size_t> w{1, 1000};
    dagpp::csr::wdigraph_builder<node_t, std::size_t> b;
    b.reserve_nodes(n);
    b.reserve_edges(n * avg_out);
    for (std::size_t i = 0; i < n; ++i) b.add_node(make_node(i));
    for (std::size_t i = 0; i + 1 < n; ++i) {
        const std::size_t remaining = n - i - 1;
        const std::size_t out = std::min(avg_out, remaining);
        std::vector<std::size_t> targets(remaining);
        std::iota(targets.begin(), targets.end(), i + 1);
        for (std::size_t k = 0; k < out; ++k) {
            std::uniform_int_distribution pick{k, remaining - 1};
            std::swap(targets[k], targets[pick(rng)]);
            b.add_edge(i, targets[k], w(rng));
        }
    }
    return b.compile();
}

static dagpp::csr::wdigraph<node_t, std::size_t>
make_sparse_dag_n_edges(const std::size_t target_edges, const std::size_t avg_out = 12) {
    const std::size_t n = std::max<std::size_t>(2, target_edges / avg_out);
    return make_sparse_dag_weighted(n, avg_out);
}

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

static void BM_ConstructChain(benchmark::State& state) {
    const auto n = static_cast<std::size_t>(state.range(0));
    for (auto _ : state)
        benchmark::DoNotOptimize(make_chain(n));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_ConstructChain)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_ConstructBinaryTree(benchmark::State& state) {
    const auto n = static_cast<std::size_t>(state.range(0));
    for (auto _ : state)
        benchmark::DoNotOptimize(make_binary_tree(n));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_ConstructBinaryTree)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_ConstructDenseDag(benchmark::State& state) {
    const auto n = static_cast<std::size_t>(state.range(0));
    for (auto _ : state)
        benchmark::DoNotOptimize(make_dense_dag(n));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() *
                            static_cast<int64_t>(n * (n - 1) / 2));
}
BENCHMARK(BM_ConstructDenseDag)->RangeMultiplier(4)->Range(64, 8192)->Complexity();

// ---------------------------------------------------------------------------
// Edge traversal
// ---------------------------------------------------------------------------

static void BM_OutEdgesChain(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    std::size_t sink = 0;
    for (auto _ : state)
        for (dagpp::nodeid_t i = 0; i < graph.node_count(); ++i)
            if (auto e = graph.out_edges(i))
                for (auto v : *e) { benchmark::DoNotOptimize(v); sink += v; }
    benchmark::DoNotOptimize(sink);
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_OutEdgesChain)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_InEdgesChain(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    std::size_t sink = 0;
    for (auto _ : state)
        for (dagpp::nodeid_t i = 0; i < graph.node_count(); ++i)
            if (auto e = graph.in_edges(i))
                for (auto v : *e) { benchmark::DoNotOptimize(v); sink += v; }
    benchmark::DoNotOptimize(sink);
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_InEdgesChain)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_OutEdgesDense(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_dense_dag(n);
    std::size_t sink = 0;
    for (auto _ : state)
        for (dagpp::nodeid_t i = 0; i < graph.node_count(); ++i)
            if (auto e = graph.out_edges(i))
                for (auto v : *e) { benchmark::DoNotOptimize(v); sink += v; }
    benchmark::DoNotOptimize(sink);
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() *
                            static_cast<int64_t>(n * (n - 1) / 2));
}
BENCHMARK(BM_OutEdgesDense)->RangeMultiplier(4)->Range(64, 8192)->Complexity();

// ---------------------------------------------------------------------------
// IsAcyclic
// ---------------------------------------------------------------------------

static void BM_IsAcyclicTrue(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(graph.is_acyclic());
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_IsAcyclicTrue)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_IsAcyclicFalse(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_cyclic(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(graph.is_acyclic());
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_IsAcyclicFalse)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_IsAcyclicDense(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_dense_dag(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(graph.is_acyclic());
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_IsAcyclicDense)->RangeMultiplier(4)->Range(64, 8192)->Complexity();

// ---------------------------------------------------------------------------
// TopoSort
// ---------------------------------------------------------------------------

static void BM_TopoSortChain(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(dagpp::topo_sort(graph));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_TopoSortChain)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_TopoSortBinaryTree(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_binary_tree(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(dagpp::topo_sort(graph));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_TopoSortBinaryTree)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_TopoSortDense(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_dense_dag(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(dagpp::topo_sort(graph));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_TopoSortDense)->RangeMultiplier(4)->Range(64, 8192)->Complexity();

// ---------------------------------------------------------------------------
// Node access
// ---------------------------------------------------------------------------

static void BM_NodeAccessSequential(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    std::size_t sink = 0;
    for (auto _ : state)
        for (dagpp::nodeid_t i = 0; i < graph.node_count(); ++i)
            sink += graph.node(i).name.size();
    benchmark::DoNotOptimize(sink);
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_NodeAccessSequential)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_NodeAccessRandom(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);

    std::vector<dagpp::nodeid_t> indices(n);
    std::iota(indices.begin(), indices.end(), dagpp::nodeid_t{0});
    std::mt19937_64 rng{std::random_device{}()};
    std::ranges::shuffle(indices, rng);

    std::size_t sink = 0;
    for (auto _ : state)
        for (const auto i : indices)
            sink += graph.node(i).name.size();
    benchmark::DoNotOptimize(sink);
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_NodeAccessRandom)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

// ---------------------------------------------------------------------------
// Dijkstra — single-source, all destinations
// ---------------------------------------------------------------------------

static void BM_DijkstraChain(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain_weighted(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(dagpp::dijkstra(graph, 0));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_DijkstraChain)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_DijkstraBinaryTree(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_binary_tree_weighted(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(dagpp::dijkstra(graph, 0));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_DijkstraBinaryTree)->RangeMultiplier(4)->Range(64, 65536)->Complexity();

static void BM_DijkstraDense(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_dense_dag_weighted(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(dagpp::dijkstra(graph, 0));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_DijkstraDense)->RangeMultiplier(2)->Range(64, 8192)->Complexity();

static void BM_DijkstraSparse(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_sparse_dag_weighted(n);
    for (auto _ : state)
        benchmark::DoNotOptimize(dagpp::dijkstra(graph, 0));
    state.SetComplexityN(static_cast<int64_t>(n));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(n));
}
BENCHMARK(BM_DijkstraSparse)->RangeMultiplier(4)->Range(1024, 65536)->Complexity();

static void BM_DijkstraSparse_PointToPoint(benchmark::State& state) {
    const auto edge_count = static_cast<std::size_t>(state.range(0));
    const auto graph      = make_sparse_dag_n_edges(edge_count);
    for (auto _ : state)
        benchmark::DoNotOptimize(dagpp::dijkstra(graph, 0));
    state.SetComplexityN(static_cast<int64_t>(edge_count));
    state.SetItemsProcessed(state.iterations() * static_cast<int64_t>(edge_count));
}
BENCHMARK(BM_DijkstraSparse_PointToPoint)->RangeMultiplier(4)->Range(256, 1 << 20)->Complexity();

// ---------------------------------------------------------------------------

BENCHMARK_MAIN();