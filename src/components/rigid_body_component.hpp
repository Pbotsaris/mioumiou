#ifndef RIGID_BODY_COMPONENT_H
#define RIGID_BODY_COMPONENT_H

#include <glm.hpp>
#include "utils/constants.hpp"

struct RigidBodyComponent {
  glm::vec2 velocity ;

   explicit RigidBodyComponent(glm::vec2 vel = constants::Defaults::RigidBody::VELOCITY) // NOLINT
      : velocity(vel) {}
};

#endif
