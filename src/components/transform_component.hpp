#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <fwd.hpp>
#include <glm.hpp>

struct TransformComponent {

  glm::vec2 position;
  glm::vec2 scale;
  glm::float64_t rotation;

  explicit TransformComponent(glm::vec2 pos = glm::vec2(0, 0), //NOLINT: args with similar types
                              glm::vec2 scl = glm::vec2(1, 1),
                              glm::float64_t rot = 0)
      : position(pos), scale(scl), rotation(rot) {}
};

#endif
