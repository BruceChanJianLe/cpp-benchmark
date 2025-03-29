#include "benchmark/benchmark.h"
#include <vector>
#include <list>
#include <cstddef>
#include <memory_resource>

static void NoopBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(0);
  }
}

static void PMRList(benchmark::State& state)
{
  for (auto _ : state) {
    std::byte stack_buff[2048];
    std::pmr::monotonic_buffer_resource rsrc(stack_buff, sizeof stack_buff);
    std::pmr::list<int> list_of_things1{{1,2,3,4,5,6,7,8,9,10}, &rsrc};
    std::pmr::list<int> list_of_things2{{1,2,3,4,5,6,7,8,9,10}, &rsrc};
    benchmark::DoNotOptimize(list_of_things1);
    benchmark::DoNotOptimize(list_of_things2);
  }
}

static void StdList(benchmark::State& state)
{
  for (auto _ : state) {
    std::list<int> list_of_things1{{1,2,3,4,5,6,7,8,9,10}};
    std::list<int> list_of_things2{{1,2,3,4,5,6,7,8,9,10}};
    benchmark::DoNotOptimize(list_of_things1);
    benchmark::DoNotOptimize(list_of_things2);
  }
}

static void PMRVector(benchmark::State& state)
{
  for (auto _ : state) {
    std::byte stack_buff[2048];
    std::pmr::monotonic_buffer_resource rsrc(stack_buff, sizeof stack_buff);
    std::pmr::vector<int> vector_of_things1{{1,2,3,4,5,6,7,8,9,10}, &rsrc};
    std::pmr::vector<int> vector_of_things2{{1,2,3,4,5,6,7,8,9,10}, &rsrc};
    benchmark::DoNotOptimize(vector_of_things1);
    benchmark::DoNotOptimize(vector_of_things2);
  }
}

static void StdVector(benchmark::State& state)
{
  for (auto _ : state) {
    std::vector<int> vector_of_things1{{1,2,3,4,5,6,7,8,9,10}};
    std::vector<int> vector_of_things2{{1,2,3,4,5,6,7,8,9,10}};
    benchmark::DoNotOptimize(vector_of_things1);
    benchmark::DoNotOptimize(vector_of_things2);
  }
}
// Register the benchmarks
BENCHMARK(NoopBenchmark); // Base line

BENCHMARK(PMRList);
BENCHMARK(StdList);

BENCHMARK(PMRVector);
BENCHMARK(StdVector);

// Run the benchmark
BENCHMARK_MAIN();
