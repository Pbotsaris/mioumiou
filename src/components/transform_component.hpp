#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <fwd.hpp>
#include <glm.hpp>
#include "utils/constants.hpp"

struct TransformComponent {

  glm::vec2      position; // x, y in pixels
  glm::vec2      scale; // multiplier
  glm::float64_t rotation; // in radiants

  explicit TransformComponent(glm::vec2 pos      = constants::Defaults::Transform::POSITION, //NOLINT: args with similar types
                              glm::vec2 scl      = constants::Defaults::Transform::SCALE,
                              glm::float64_t rot = constants::Defaults::Transform::ROTATION
                              )
      : position(pos), scale(scl), rotation(rot) {}
};

#endif
