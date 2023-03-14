#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <fwd.hpp>
#include <glm.hpp>

struct TransformComponent {

  glm::vec2      position; // x, y in pixels
  glm::vec2      scale; // multiplier
  glm::float64_t rotation; // in radiants

  explicit TransformComponent(glm::vec2 pos      = {0, 0}, //NOLINT: args with similar types
                              glm::vec2 scl      = {1, 1},
                              glm::float64_t rot = 0.0
                              )
      : position(pos), scale(scl), rotation(rot) {}
};

#endif
