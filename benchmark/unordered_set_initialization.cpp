#include "benchmark/benchmark.h"

#include <cstddef>
#include <random>
#include <vector>
#include <unordered_set>

static std::vector<int> generate_data(std::size_t N) {
  std::mt19937_64 rng(12345);
  std::uniform_int_distribution<int> dist(0, 1'000'000'000);

  std::vector<int> data;
  data.reserve(N);
  for (std::size_t i = 0u; i < N; ++i) data.push_back(dist(rng));

  return data;
}

static void NoopBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(0);
  }
}

// No optimization
static void BM_UnorderedSet_NoOpt(benchmark::State& state) {
  auto nums = generate_data(state.range(0));

  for (auto _ : state) {
    std::unordered_set<int> records;
    records.insert(nums.begin(), nums.end());
    benchmark::DoNotOptimize(records);
  }
}

static void BM_UnorderedSet_DirectConstruct(benchmark::State& state) {
  auto nums = generate_data(state.range(0));

  for (auto _ : state) {
    std::unordered_set<int> records(nums.begin(), nums.end());
    benchmark::DoNotOptimize(records);
  }
}

static void BM_UnorderedSet_BucketCountConstruct(benchmark::State& state) {
  auto nums = generate_data(state.range(0));

  for (auto _ : state) {
    std::unordered_set<int> records(nums.size());
    records.insert(nums.begin(), nums.end());
    benchmark::DoNotOptimize(records);
  }
}

static void BM_UnorderedSet_ReserveAndInsert(benchmark::State& state) {
  auto nums = generate_data(state.range(0));

  for (auto _ : state) {
    std::unordered_set<int> records;
    records.reserve(nums.size());
    records.insert(nums.begin(), nums.end());
    benchmark::DoNotOptimize(records);
  }
}

// Register the benchmarks
BENCHMARK(NoopBenchmark); // Base line

// Construct (default) -> insert
BENCHMARK(BM_UnorderedSet_NoOpt)
  ->Arg(1000000)    // 1M
  ->Arg(3000000)    // 3M
  ->Arg(10000000);  // 10M

// Construct (insert)
BENCHMARK(BM_UnorderedSet_DirectConstruct)
  ->Arg(1000000)
  ->Arg(3000000)
  ->Arg(10000000);

// Construct (with bucket info) -> insert
BENCHMARK(BM_UnorderedSet_BucketCountConstruct)
  ->Arg(1000000)
  ->Arg(3000000)
  ->Arg(10000000);

// Construct -> reserve -> insert
BENCHMARK(BM_UnorderedSet_ReserveAndInsert)
  ->Arg(1000000)
  ->Arg(3000000)
  ->Arg(10000000);

// Run the benchmark
BENCHMARK_MAIN();
