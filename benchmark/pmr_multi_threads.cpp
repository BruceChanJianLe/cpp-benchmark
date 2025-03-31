#include "benchmark/benchmark.h"
#include <cstddef>
#include <list>
#include <memory_resource>

static void NoopBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(0);
  }
}

template <typename Allocator>
static void CreateAndAccess(benchmark::State &state)
{
  auto worker = []() {
    Allocator alloc;
    std::pmr::list<int> list(alloc.get_resource());

    for (auto i = 0; i < 100000; ++i) {
      list.push_back(i);
    }

    for (auto i = 0; i < 1000; ++i) {
      for (auto &val : list) {
        ++val;
      }
      list.erase(list.begin());
      list.push_back(0);
    }

    return list.back();
  };

  for (auto _ : state) {
    auto result = worker();
    benchmark::DoNotOptimize(result);
  }
}

struct NewDelete
{
  std::pmr::memory_resource *get_resource() {
    return std::pmr::new_delete_resource();
  }
};

template <std::size_t Size>
struct Monotonic
{
  std::pmr::monotonic_buffer_resource memory_resource{ Size };
  std::pmr::polymorphic_allocator<> alloc{ &memory_resource };

  template<typename Container>
  auto create(const auto &data_source)
  {
    if constexpr (requires { Container{}.bucket_count(); }) {
      return Container{ data_source.begin(), data_source.end(), Container{}.bucket_count(), alloc};
    } else {
      return Container{ data_source.begin(), data_source().end(), alloc };
    }
  }

  template <typename Container>
  auto copy(const Container &container) {
    return Container{container, alloc};
  }

  std::pmr::memory_resource *get_resource() {
    return &memory_resource;
  }
};

template <std::size_t Size>
struct PoolMonotonic
{
  std::pmr::monotonic_buffer_resource upstream{ Size };
  std::pmr::unsynchronized_pool_resource memory_resource{ &upstream };
  std::pmr::polymorphic_allocator<> alloc{ &memory_resource };

  template <typename Container>
  auto create(const auto &data_source) {
    if constexpr (requires { Container{}.bucket_count(); }) {
      return Container{ data_source.begin(), data_source.end(), Container{}.bucket_count(), alloc};
    } else {
      return Container{ data_source.begin(), data_source.end(), alloc };
    }
  }

  template <typename Container>
  auto copy(const Container &container) {
    return Container{container, alloc};
  }

  std::pmr::memory_resource* get_resource() {
    return &memory_resource;
  }
};

template <std::size_t Size>
struct MonotonicWinkOut
{
  std::pmr::monotonic_buffer_resource memory_resource{ Size };
  std::pmr::polymorphic_allocator<> alloc{ &memory_resource };

  template <typename Container>
  auto create(const auto &data_source) {
    if constexpr (requires { Container{}.bucket_count(); }) {
      return alloc.new_object<Container>(data_source.begin(), data_source.end(), Container{}.bucket_count(), alloc);
    } else {
      return alloc.new_object<Container>(data_source.begin(), data_source.end(), alloc);
    }
  }
};


// Register the benchmarks
BENCHMARK(NoopBenchmark); // Base line

BENCHMARK_TEMPLATE(CreateAndAccess, NewDelete)->Threads(1);
BENCHMARK_TEMPLATE(CreateAndAccess, Monotonic<10>)->Threads(1);
BENCHMARK_TEMPLATE(CreateAndAccess, PoolMonotonic<10>)->Threads(1);

BENCHMARK_TEMPLATE(CreateAndAccess, NewDelete)->Threads(3);
BENCHMARK_TEMPLATE(CreateAndAccess, Monotonic<10>)->Threads(3);
BENCHMARK_TEMPLATE(CreateAndAccess, PoolMonotonic<10>)->Threads(3);

BENCHMARK_TEMPLATE(CreateAndAccess, NewDelete)->Threads(5);
BENCHMARK_TEMPLATE(CreateAndAccess, Monotonic<10>)->Threads(5);
BENCHMARK_TEMPLATE(CreateAndAccess, PoolMonotonic<10>)->Threads(5);

BENCHMARK_TEMPLATE(CreateAndAccess, NewDelete)->Threads(7);
BENCHMARK_TEMPLATE(CreateAndAccess, Monotonic<10>)->Threads(7);
BENCHMARK_TEMPLATE(CreateAndAccess, PoolMonotonic<10>)->Threads(7);

BENCHMARK_TEMPLATE(CreateAndAccess, NewDelete)->Threads(10);
BENCHMARK_TEMPLATE(CreateAndAccess, Monotonic<10>)->Threads(10);
BENCHMARK_TEMPLATE(CreateAndAccess, PoolMonotonic<10>)->Threads(10);

BENCHMARK_TEMPLATE(CreateAndAccess, NewDelete)->Threads(16);
BENCHMARK_TEMPLATE(CreateAndAccess, Monotonic<10>)->Threads(16);
BENCHMARK_TEMPLATE(CreateAndAccess, PoolMonotonic<10>)->Threads(16);

// Run the benchmark
BENCHMARK_MAIN();

/// ./build/benchmark/pmr_multi_threads --benchmark_repetitions=4
