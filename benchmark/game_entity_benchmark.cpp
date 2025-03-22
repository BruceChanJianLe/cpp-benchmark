#include "dod/game_entity.hpp"
#include "oop/game_entity.hpp"

#include "benchmark/benchmark.h"
#include <vector>

static void NoopBenchmark(benchmark::State& state) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(0);
  }
}

// OOP Benchmark
static void OOPGameEntityUpdateSpeed(benchmark::State& state) {
  auto entities = std::vector<oop::Entity>(state.range(0));

  for (auto _ : state) {
    for (auto& entity : entities) {
      entity.updateLinearSpeedX();
    }
    benchmark::DoNotOptimize(entities);
  }
}

static void OOPGameEntityEnableRender(benchmark::State& state) {
  auto entities = std::vector<oop::Entity>(state.range(0));

  for (auto _ : state) {
    for (auto& entity : entities) {
      entity.enableRender();
    }
    benchmark::DoNotOptimize(entities);
  }
}

// DOD Benchmark
static void DODGameEntityUpdateSpeed(benchmark::State& state) {
  auto entities = dod::TransformEntity(state.range(0));

  for (auto _ : state) {
    for (auto& transform : entities.transforms) {
      ++transform.linear.x;
    }
    benchmark::DoNotOptimize(entities);
  }
}

static void DODGameEntityEnableRender(benchmark::State& state) {
  auto entities = dod::RenderEntity(state.range(0));

  for (auto _ : state) {
    for (std::vector<bool>::reference renderable : entities.renderables) {
      renderable = true;
    }
    benchmark::DoNotOptimize(entities);
  }
}

// Register the benchmarks
BENCHMARK(NoopBenchmark); // Base line
BENCHMARK(OOPGameEntityUpdateSpeed)->Range(8, 1 << 16);
BENCHMARK(OOPGameEntityEnableRender)->Range(8, 1 << 16);
BENCHMARK(DODGameEntityUpdateSpeed)->Range(8, 1 << 16);
BENCHMARK(DODGameEntityEnableRender)->Range(8, 1 << 16);

// Run the benchmark
BENCHMARK_MAIN();
