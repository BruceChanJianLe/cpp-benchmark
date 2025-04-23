#include "benchmark/benchmark.h"
#include <algorithm>
#include <execution>
#include <random>
#include <ranges>
#include <stdexcept>

#include "experimental/__p0009_bits/mdspan.hpp"
#include <concepts>

static constexpr std::size_t rows{100'00};
static constexpr std::size_t cols{100'00};

namespace stdex = std::experimental;

// Helper function: Initialize a big matrix
std::vector<std::vector<double>> create_matrix() {
  std::vector<std::vector<double>> mat(rows, std::vector<double>(cols));
  std::mt19937 gen(42);
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  for (auto& row : mat) {
    for (auto& elem : row) {
      elem = dist(gen);
    }
  }

  return mat;
}

// Create contiguous memory layout object
// A better approach

template<typename T>
concept AllowedMatrixType = 
    std::same_as<std::remove_cv_t<T>, float> ||
    std::same_as<std::remove_cv_t<T>, double> ||
    std::same_as<std::remove_cv_t<T>, int>;

template <AllowedMatrixType T>
class Matrix {
public:
  using element_type = T;
  using extends_type = stdex::dextents<std::size_t, 2>;
  using mdspan_type = stdex::mdspan<element_type, extends_type>;

  Matrix(const std::size_t _rows, const std::size_t _cols)
    : data_(_rows * _cols), view_{data_.data(), _rows, _cols}
    , rows_{_rows} , cols_{_cols}
  {
    if (rows_ == 0 || cols_ == 0) {
      throw std::invalid_argument("Matrix dimensions must be greater than zero.");
    }
  }

  // Access element
  T& operator() (const std::size_t _row, const std::size_t _col) {
    return view_[_row, _col];
  }
  const T& operator() (const std::size_t _row, const std::size_t _col) const {
    return view_[_row, _col];
  }

  // Access element safely
  T& at(const std::size_t _row, const std::size_t _col) {
    if (_row >= rows() || _col >= cols()) {
      throw std::out_of_range("Matrix access out of range!");
    }
    return view_[_row, _col];
  }
  const T& at(const std::size_t _row, const std::size_t _col) const {
    if (_row >= rows() || _col >= cols()) {
      throw std::out_of_range("Matrix access out of range!");
    }
    return view_[_row, _col];
  }

  [[nodiscard]] std::size_t rows() const { return rows_; }
  [[nodiscard]] std::size_t cols() const { return cols_; }

  mdspan_type view() { return view_; }
  const mdspan_type view() const { return view_; }

  // Access underlying data structure
  std::vector<T>& data() { return data_; }
  const std::vector<T>& data() const { return data_; }

private:
  std::vector<T> data_;
  mdspan_type view_;
  std::size_t rows_;
  std::size_t cols_;
};

// Helper function: Initialize a my big matrix
Matrix<double> create_my_matrix() {
  Matrix<double> mat(rows, cols);
  std::mt19937 gen(42);
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  for (auto i = 0uz; i < mat.rows(); ++i) {
    for (auto j = 0uz; j < mat.cols(); ++j) {
      mat(i, j) = dist(gen);
    }
  }

  return mat;
}

static void TraditionalNestedLoop(benchmark::State& state) {
  auto mat = create_matrix();

  for (auto _ : state) {
    for (auto& row : mat) {
      for (auto& elem : row) {
        elem *= 2.0;
      }
    }
    benchmark::DoNotOptimize(mat);
  }
}
BENCHMARK(TraditionalNestedLoop);

// Parallelize only outter loop
static void ParallelOutterLoop(benchmark::State& state) {
  auto mat = create_matrix();
  // Lazily evaluated, much slower actually
  // auto indexes = std::views::iota(0uz, mat.size());

  for (auto _ : state) {
    std::vector<std::size_t> indexes(mat.size());
    std::iota(indexes.begin(), indexes.end(), 0);

    std::for_each(std::execution::par_unseq,
      indexes.begin(), indexes.end(),
      [&mat](const std::size_t index) {
        for (auto& elem : mat[index]) {
          elem *= 2.0;
        }
      }
    );

    benchmark::DoNotOptimize(mat);
  }
}
BENCHMARK(ParallelOutterLoop);

// Parallelize inner and outter loop (generally worse)
// PREFER Single parallelization
static void ParallelInnerOutterLoop(benchmark::State& state) {
  auto mat = create_matrix();
  // Lazily evaluated, much slower actually
  // auto indexes = std::views::iota(0uz, mat.size());

  for (auto _ : state) {
    std::vector<std::size_t> indexes(mat.size());
    std::iota(indexes.begin(), indexes.end(), 0);

    std::for_each(std::execution::par_unseq,
      indexes.begin(), indexes.end(),
      [&mat](const std::size_t index) {
        std::vector<std::size_t> inner_indexes(mat.size());
        std::iota(inner_indexes.begin(), inner_indexes.end(), 0);
        auto row = mat[index];

        std::for_each(std::execution::par_unseq,
          inner_indexes.begin(), inner_indexes.end(),
          [&row](const std::size_t inner_index) {
            row[inner_index] *= 2.0;
          }
        );
      }
    );

    benchmark::DoNotOptimize(mat);
  }
}
BENCHMARK(ParallelInnerOutterLoop);


// Flatten and parallelize (worst as it is lazily evaluated)
// Note that, this creates another copy of flattened vector
// hence, the changes will not reflect on mat
static void FlattenAndParallel(benchmark::State& state) {
  auto mat = create_matrix();

  for (auto _ : state) {
    // Lazy evaluation, make things slower
    auto flattened = mat | std::views::join;

    std::for_each(std::execution::par_unseq,
      flattened.begin(), flattened.end(),
      [](auto& elem) {
          elem *= 2.0;
      }
    );

    benchmark::DoNotOptimize(mat);
    benchmark::DoNotOptimize(flattened);
  }
}
BENCHMARK(FlattenAndParallel);

// Use a memory friendly layout
// But leverage mdspan for matrix like structure
static void MemoryFriendlyFlattenAndParallel(benchmark::State& state) {
  auto mat = create_my_matrix();

  for (auto _ : state) {
    auto& data = mat.data();

    std::for_each(std::execution::par_unseq,
        data.begin(), data.end(),
        [](auto& elem) {
          elem *= 2.0;
        }
    );

    benchmark::DoNotOptimize(mat);
  }
}
BENCHMARK(MemoryFriendlyFlattenAndParallel);

BENCHMARK_MAIN();
