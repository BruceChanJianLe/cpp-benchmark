#pragma once

#include "common/defs.hpp"
#include <vector>

namespace dod
{
  struct TransformEntity
  {
    TransformEntity(const int size);

    std::vector<Transform> transforms;
  };

  struct PhysicsEntity
  {
    PhysicsEntity(const int size);

    std::vector<Physics> physics;
  };

  struct RenderEntity
  {
    RenderEntity(const int size);

    std::vector<bool> renderables;
  };
} // dod
