#include <cstddef>
#include <print>
#include <type_traits>

bool same_bytes(const std::byte *p0, const std::byte *p1, std::size_t n) {
  for (std::size_t i = 0u; i < n; ++i) {
    if (*(p0 + i) != *(p1 + i)) {
      return false;
    }
  }
  return true;
}

template <typename T, typename U> bool same_bytes(const T &t, const U &u) {
  static_assert(sizeof t == sizeof u);
  static_assert(std::has_unique_object_representations_v<T>);
  static_assert(std::has_unique_object_representations_v<U>);

  return same_bytes(reinterpret_cast<const std::byte *>(&t),
                    reinterpret_cast<const std::byte *>(&u), sizeof t);
}

/**
 * std::has_unique_object_representations_v trait is true for types uniquely
 * defined by their values, that is, types exempt of padding bits...
 * See bool b below, will not compile as it is not uniquely represented object
 *
 */

struct X {
  // bool b{false}; // This will not compile, padding involves
  int x{2}, y{3};
};

struct Y {
  // bool b{false}; // This will not compile, padding involves
  int x{2}, y{3};
};

struct X1 {
  int x{2}, y{3}, z{9};
};

struct Y1 {
  int x{2}, y{3}, z{8};
};

#include <cassert>

int main ([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  constexpr X x;
  constexpr Y y;
  std::println("X and Y {}", same_bytes(x, y) ? "is the same!" : "is not the same!");

  constexpr X1 x1;
  constexpr Y1 y1;
  std::println("X1 and Y1 {}", same_bytes(x1, y1) ? "is the same!" : "is not the same!");
}
