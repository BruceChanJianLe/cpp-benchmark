#pragma once

#include <cstddef>

struct Linear
{
  Linear() : x{0}, y{0}, z{0} {}

  int x;
  int y;
  int z;
};

struct Angular
{
  Angular() : x{0}, y{0}, z{0}, w{0} {}

  int x;
  int y;
  int z;
  int w;
};

struct Transform
{
  Transform() : linear{}, angular{} {}

  Linear linear;
  Angular angular;
};

struct Physics
{
  Physics() : option{0} {}

  std::size_t option;
};
