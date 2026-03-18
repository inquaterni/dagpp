//
// Google Benchmark suite for dag++
//
#include <benchmark/benchmark.h>
#include <malloc.h>   // mallinfo2 (Linux/glibc >= 2.33)
#include <numeric>
#include <random>
#include <string>
#include <format>

#include "dagpp.h"
#include "dot.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// A realistic node type: two heap-allocated strings simulate real-world
// payloads (e.g. task names, build-system targets, dependency labels).
struct node_t {
    std::string name;   // e.g. "task_0042"
    std::string label;  // e.g. "build:lib:task_0042"
};

static node_t make_node(std::size_t i) {
    return node_t{
        std::format("task_{:04d}", i),
        std::format("build:lib:task_{:04d}", i)
    };
}

// ---------------------------------------------------------------------------
// Memory helpers
// ---------------------------------------------------------------------------

// Returns current glibc heap usage in bytes (uordblks = in-use allocated bytes).
static std::size_t heap_used() noexcept {
    return mallinfo2().uordblks;
}

// Measures the *net* heap bytes allocated by calling fn().
// The result object is immediately destroyed so its memory is freed;
// we capture the high-water mark before destruction.
template<typename Fn>
static std::size_t heap_delta(Fn&& fn) {
    const auto before = heap_used();
    { auto obj = fn(); benchmark::DoNotOptimize(obj); }
    const auto after_live = heap_used();
    const auto b2 = heap_used();
    { volatile auto obj2 = fn(); (void)obj2; }
    const auto peak = heap_used();
    (void)before; (void)after_live; (void)b2;
    const auto b3 = heap_used();
    auto g = fn();
    const auto a3 = heap_used();
    benchmark::DoNotOptimize(g);
    return a3 >= b3 ? a3 - b3 : 0;
}

// Simpler version for callables that return void or whose allocation we
// measure while the enclosing graph (already built) stays alive.
static std::size_t heap_delta_algo(auto fn) {
    const auto before = heap_used();
    { auto r = fn(); benchmark::DoNotOptimize(r); }
    const auto after = heap_used();
    // Algorithms free their temporaries; delta may be 0 or negative due to
    // allocator bookkeeping noise – clamp to 0.
    return after >= before ? after - before : 0;
}

static void set_mem_counter(benchmark::State& state,
                            const char* name,
                            const std::size_t bytes) {
    state.counters[name] = benchmark::Counter(
        static_cast<double>(bytes),
        benchmark::Counter::kDefaults,
        benchmark::Counter::OneK::kIs1024
    );
}

// ---------------------------------------------------------------------------
// Graph factories
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
        const std::size_t l = 2 * i + 1, r = 2 * i + 2;
        if (l < n) b.add_edge(i, l);
        if (r < n) b.add_edge(i, r);
    }
    return b.compile();
}

static dagpp::csr::digraph<node_t> make_dense_dag(std::size_t n) {
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
// BM_GraphConstruct  – measures csr::digraph_builder::compile()
// GraphMem = heap footprint of the compiled graph (nodes + edge lists)
// ---------------------------------------------------------------------------

static void BM_ConstructChain(benchmark::State& state) {
    const auto n = static_cast<std::size_t>(state.range(0));
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_chain(n); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(make_chain(n));
    }
    state.SetItemsProcessed(state.iterations() *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_ConstructChain)->RangeMultiplier(4)->Range(64, 65536);

static void BM_ConstructBinaryTree(benchmark::State& state) {
    const auto n = static_cast<std::size_t>(state.range(0));
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_binary_tree(n); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(make_binary_tree(n));
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_ConstructBinaryTree)->RangeMultiplier(4)->Range(64, 65536);

static void BM_ConstructDenseDag(benchmark::State& state) {
    const auto n = static_cast<std::size_t>(state.range(0));
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_dense_dag(n); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(make_dense_dag(n));
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n * (n - 1) / 2));
}
BENCHMARK(BM_ConstructDenseDag)->RangeMultiplier(4)->Range(16, 1024);

// ---------------------------------------------------------------------------
// BM_OutEdges / BM_InEdges  – no allocation in hot path; report GraphMem only
// ---------------------------------------------------------------------------

static void BM_OutEdgesChain(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_chain(n); }));
    std::size_t sink = 0;
    for (auto _ : state) {
        for (dagpp::nodeid_t i = 0; i < graph.count(); ++i) {
            if (auto e = graph.out_edges(i); e.has_value())
                for (auto v : *e) { benchmark::DoNotOptimize(v); sink += v; }
        }
    }
    benchmark::DoNotOptimize(sink);
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_OutEdgesChain)->RangeMultiplier(4)->Range(64, 65536);

static void BM_InEdgesChain(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_chain(n); }));
    std::size_t sink = 0;
    for (auto _ : state) {
        for (dagpp::nodeid_t i = 0; i < graph.count(); ++i) {
            if (auto e = graph.in_edges(i); e.has_value())
                for (auto v : *e) { benchmark::DoNotOptimize(v); sink += v; }
        }
    }
    benchmark::DoNotOptimize(sink);
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_InEdgesChain)->RangeMultiplier(4)->Range(64, 65536);

static void BM_OutEdgesDense(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_dense_dag(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_dense_dag(n); }));
    std::size_t sink = 0;
    for (auto _ : state) {
        for (dagpp::nodeid_t i = 0; i < graph.count(); ++i) {
            if (auto e = graph.out_edges(i); e.has_value())
                for (auto v : *e) { benchmark::DoNotOptimize(v); sink += v; }
        }
    }
    benchmark::DoNotOptimize(sink);
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n * (n - 1) / 2));
}
BENCHMARK(BM_OutEdgesDense)->RangeMultiplier(4)->Range(16, 1024);

// ---------------------------------------------------------------------------
// BM_IsAcyclic
// GraphMem = graph footprint; TopoTempMem = temporary vectors inside is_acyclic()
// ---------------------------------------------------------------------------

static void BM_IsAcyclicTrue(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_chain(n); }));
    set_mem_counter(state, "TopoTempMem",
        heap_delta_algo([&]{ return graph.is_acyclic(); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(graph.is_acyclic());
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_IsAcyclicTrue)->RangeMultiplier(4)->Range(64, 65536);

static void BM_IsAcyclicFalse(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_cyclic(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_cyclic(n); }));
    set_mem_counter(state, "TopoTempMem",
        heap_delta_algo([&]{ return graph.is_acyclic(); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(graph.is_acyclic());
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_IsAcyclicFalse)->RangeMultiplier(4)->Range(64, 65536);

static void BM_IsAcyclicDense(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_dense_dag(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_dense_dag(n); }));
    set_mem_counter(state, "TopoTempMem",
        heap_delta_algo([&]{ return graph.is_acyclic(); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(graph.is_acyclic());
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_IsAcyclicDense)->RangeMultiplier(4)->Range(16, 1024);

// ---------------------------------------------------------------------------
// BM_TopoSort
// TopoTempMem = in_degree vector + result vector allocated by topo_sort()
// ---------------------------------------------------------------------------

static void BM_TopoSortChain(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_chain(n); }));
    set_mem_counter(state, "TopoTempMem",
        heap_delta_algo([&]{ return dagpp::topo_sort(graph); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(dagpp::topo_sort(graph));
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_TopoSortChain)->RangeMultiplier(4)->Range(64, 65536);

static void BM_TopoSortBinaryTree(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_binary_tree(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_binary_tree(n); }));
    set_mem_counter(state, "TopoTempMem",
        heap_delta_algo([&]{ return dagpp::topo_sort(graph); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(dagpp::topo_sort(graph));
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_TopoSortBinaryTree)->RangeMultiplier(4)->Range(64, 65536);

static void BM_TopoSortDense(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_dense_dag(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_dense_dag(n); }));
    set_mem_counter(state, "TopoTempMem",
        heap_delta_algo([&]{ return dagpp::topo_sort(graph); }));
    for (auto _ : state) {
        benchmark::DoNotOptimize(dagpp::topo_sort(graph));
    }
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_TopoSortDense)->RangeMultiplier(4)->Range(16, 1024);

// ---------------------------------------------------------------------------
// BM_NodeAccess  – GraphMem only; no per-call allocation
// ---------------------------------------------------------------------------

static void BM_NodeAccessSequential(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_chain(n); }));
    std::size_t sink = 0;
    for (auto _ : state) {
        for (dagpp::nodeid_t i = 0; i < graph.count(); ++i)
            sink += graph.node(i).name.size();
    }
    benchmark::DoNotOptimize(sink);
    state.SetItemsProcessed(static_cast<int64_t>(state.iterations()) *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_NodeAccessSequential)->RangeMultiplier(4)->Range(64, 65536);

static void BM_NodeAccessRandom(benchmark::State& state) {
    const auto n     = static_cast<std::size_t>(state.range(0));
    const auto graph = make_chain(n);
    set_mem_counter(state, "GraphMem", heap_delta([n]{ return make_chain(n); }));

    std::vector<dagpp::nodeid_t> indices(n);
    std::iota(indices.begin(), indices.end(), dagpp::nodeid_t{0});
    const std::random_device rd;
    std::mt19937 rng{static_cast<std::mersenne_twister_engine<unsigned long, 32, 624, 397, 31, 2567483615, 11, 4294967295, 7, 2636928640, 15, 4022730752, 18, 1812433253>::result_type>(rd.entropy())};
    std::ranges::shuffle(indices, rng);

    std::size_t sink = 0;
    for (auto _ : state) {
        for (const auto i : indices)
            sink += graph.node(i).name.size();
    }
    benchmark::DoNotOptimize(sink);
    state.SetItemsProcessed(state.iterations() *
                            static_cast<int64_t>(n));
}
BENCHMARK(BM_NodeAccessRandom)->RangeMultiplier(4)->Range(64, 65536);

// ---------------------------------------------------------------------------

BENCHMARK_MAIN();
