#include "benchmark/benchmark.h"
#include <execution>

constexpr size_t N = 100'000'000;

static void ForEachRaw(benchmark::State& state) {
    std::vector<int> v(N, 1);
    for (auto _ : state) {
        std::for_each(v.begin(), v.end(), [](int& x) { x += 1; });
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ForEachRaw);

static void ForEachSeq(benchmark::State& state) {
    std::vector<int> v(N, 1);
    for (auto _ : state) {
        std::for_each(std::execution::seq, v.begin(), v.end(), [](int& x) { x += 1; });
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ForEachSeq);

static void ForEachUnSeq(benchmark::State& state) {
  std::vector<int> v(N, 1);
  for (auto _ : state) {
    std::for_each(std::execution::unseq, v.begin(), v.end(), [](int& x) { x += 1; });
    benchmark::DoNotOptimize(v);
  }
}
BENCHMARK(ForEachUnSeq);

static void ForEachPar(benchmark::State& state) {
    std::vector<int> v(N, 1);
    for (auto _ : state) {
        std::for_each(std::execution::par, v.begin(), v.end(), [](int& x) { x += 1; });
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ForEachPar);

static void ForEachParUnseq(benchmark::State& state) {
    std::vector<int> v(N, 1);
    for (auto _ : state) {
        std::for_each(std::execution::par_unseq, v.begin(), v.end(), [](int& x) { x += 1; });
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ForEachParUnseq);

BENCHMARK_MAIN();
