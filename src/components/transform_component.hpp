#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <fwd.hpp>
#include <glm.hpp>

struct TransformComponent {

  glm::vec2      position  = glm::vec2(0, 0);
  glm::vec2      scale     =  glm::vec2(1, 1);
  glm::float64_t rotation  = glm::float64_t(0.0);

  TransformComponent() = default;

  explicit TransformComponent(glm::vec2 pos, //NOLINT: args with similar types
                              glm::vec2 scl,
                              glm::float64_t rot)
      : position(pos), scale(scl), rotation(rot) {}
};

#endif
