#ifndef RIGID_BODY_COMPONENT_H
#define RIGID_BODY_COMPONENT_H

#include <glm.hpp>

struct RigidBodyComponent {
  glm::vec2 velocity;

  explicit RigidBodyComponent(glm::vec2 vel = glm::vec2(0.0, 0.0))
      : velocity(vel) {}
};

#endif
