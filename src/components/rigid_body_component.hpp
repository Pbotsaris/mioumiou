#ifndef RIGID_BODY_COMPONENT_H
#define RIGID_BODY_COMPONENT_H

#include <glm.hpp>

struct RigidBodyComponent {
  glm::vec2 velocity = glm::vec2(0.0, 0.0);

  RigidBodyComponent() = default;

  explicit RigidBodyComponent(glm::vec2 vel )
      : velocity(vel) {}
};

#endif
