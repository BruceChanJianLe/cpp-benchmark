#include "dod/game_entity.hpp"

namespace dod
{
  TransformEntity::TransformEntity(const int size)
  {
    transforms.reserve(size);
  }

  PhysicsEntity::PhysicsEntity(const int size)
  {
    physics.reserve(size);
  }

  RenderEntity::RenderEntity(const int size)
  {
    renderables.reserve(size);
  }
} // dod
