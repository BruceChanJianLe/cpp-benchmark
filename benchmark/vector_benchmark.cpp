#include "benchmark/benchmark.h"
#include <vector>
#include <cstddef>

static void NoopBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(0);
  }
}

static void VecPushBack(benchmark::State& state) {
  const auto size = state.range(0);

  for (auto _ : state) {
    std::vector<std::byte> vec;
    for (auto i = 0; i < size + 1; ++i) {
      vec.push_back({});
    }
    benchmark::DoNotOptimize(vec);
  }
}

static void VecPushBackWithReserve(benchmark::State& state) {
  const auto size = state.range(0);

  for (auto _ : state) {
    std::vector<std::byte> vec;
    vec.reserve(size + 1);
    for (auto i = 0; i < size + 1; ++i) {
      vec.push_back({});
    }
    benchmark::DoNotOptimize(vec);
  }
}

// Register the benchmarks
BENCHMARK(NoopBenchmark); // Base line
BENCHMARK(VecPushBack)->RangeMultiplier(2)->Range(4, 32);
BENCHMARK(VecPushBack)->Arg(10000);
BENCHMARK(VecPushBackWithReserve)->RangeMultiplier(2)->Range(4, 32);
BENCHMARK(VecPushBackWithReserve)->Arg(10000);

// Run the benchmark
BENCHMARK_MAIN();
