#include <cstdint>
#include <print>
#include <string>

// An empty class, sizeof(X) is > 0
class X {
};

class Y {
  X x;
};

class X1 {
  char c;
};

class Y1 {
  X1 x1;
};


class Y2 {
  X x;
  char c;
};

class ComplexX {
  char c;
  short s;
  int i;
};


class ComplexXNotOptimize {
  short s;
  int i;
  char c;
};


int main ([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  std::println("sizeof(X) > 0: {}", (sizeof(X) > 0) ? "true" : "false");
  std::println("sizeof(Y) == sizeof(X): {}", (sizeof(Y) == sizeof(X)) ? "true" : "false");

  // At least Y2 is equal X + char
  std::println("sizeof(Y2) >= sizeof(X) + sizeof(char): {}", (sizeof(Y2) >= sizeof(X) + sizeof(char)) ? "true" : "false");
  std::println("sizeof(Y1) == sizeof(X1): {}", (sizeof(Y1) == sizeof(X1)) ? "true" : "false");

  // For complex X object (optimize layout)
  std::println("\nComplex X object (optimize layout)");
  std::println("sizeof(short) == alignof(short): {}", (sizeof(short) == alignof(short)) ? "true" : "false");
  std::println("sizeof(int) == alignof(int): {}", (sizeof(int) == alignof(int)) ? "true" : "false");
  std::println("sizeof(ComplexX) == 8: {}", (sizeof(ComplexX) == 8) ? "true" : "false"); // highly probable
  std::println("alignof(ComplexX) == alignof(int): {}", (alignof(ComplexX) == alignof(int)) ? "true" : "false"); // likewise

  // For complex X object (not optimaize layout)
  std::println("\nComplex X object (not optimize layout)");
  std::println("sizeof(short) == alignof(short): {}", (sizeof(short) == alignof(short)) ? "true" : "false");
  std::println("sizeof(int) == alignof(int): {}", (sizeof(int) == alignof(int)) ? "true" : "false");
  std::println("alignof(ComplexXNotOptimize) == alignof(int): {}", (alignof(ComplexXNotOptimize) == alignof(int)) ? "true" : "false");
  std::println("sizeof(ComplexXNotOptimize) == 12: {}", (sizeof(ComplexXNotOptimize) == 12) ? "true" : "false"); // highly probable

  return 0;
}
