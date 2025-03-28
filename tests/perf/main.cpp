#include <Spektral/Arenas/LinearArena.hpp>
#include <benchmark/benchmark.h>
#define NUM_ITERS 1000000
#define NUM_REPS 50
#define BLOCK_SIZE 40

void malloc_test(benchmark::State &state) {
  std::vector<void *> ptrs;
  for (auto _ : state) {
    auto v = malloc(BLOCK_SIZE);
    state.PauseTiming();
    ptrs.push_back(v);
  }
  for (auto p : ptrs) free(p);
}

void linear_alloc_test(benchmark::State &state) {
  Spektral::Arenas::LinearArena arena{NUM_ITERS * BLOCK_SIZE};
  for (auto _ : state) {
    auto v = arena.alloc(BLOCK_SIZE);
  }
}

BENCHMARK(malloc_test)
    ->Iterations(NUM_ITERS)
    ->Repetitions(NUM_REPS)
    ->ReportAggregatesOnly();
BENCHMARK(linear_alloc_test)
    ->Iterations(NUM_ITERS)
    ->Repetitions(NUM_REPS)
    ->ReportAggregatesOnly();
BENCHMARK_MAIN();
