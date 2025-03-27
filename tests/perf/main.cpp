#include <benchmark/benchmark.h>
#include <Spektral/Arenas/LinearArena.hpp>

void malloc_test(benchmark::State &state) {
    std::vector<void *> ptrs;
    for (auto _ : state) {
        auto v = malloc(40);
        state.PauseTiming();
        ptrs.push_back(v);
    }
}

void linear_alloc_test(benchmark::State &state) {
    Spektral::Arenas::LinearArena arena{40000000};
    std::vector<void *> ptrs;
    for (auto _ : state) {
        auto v = arena.alloc(40);
        state.PauseTiming();
        ptrs.push_back(v);
    }
}

BENCHMARK(malloc_test)->Iterations(10000)->Repetitions(50)->ReportAggregatesOnly();
BENCHMARK(linear_alloc_test)->Iterations(10000)->Repetitions(50)->ReportAggregatesOnly();
BENCHMARK_MAIN();
