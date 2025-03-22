#include "oop/game_entity.hpp"

namespace oop
{
  Entity::Entity() : transform{}, physic{}, renderable{false} {}

  void Entity::updateLinearSpeedX()
  {
    ++transform.linear.x;
  }

  void Entity::enableRender()
  {
    renderable = true;
  }

  void Entity::disableRender()
  {
    renderable = false;
  }
} // oop
