#pragma once

#include "common/defs.hpp"

namespace oop
{
  struct Entity
  {
    Entity();

    Transform transform;
    Physics physic;
    bool renderable;

    // Unncessary states
    int v0;
    int v1;
    int v2;
    int v3;

    void updateLinearSpeedX();
    void enableRender();
    void disableRender();
  };
} // oop
