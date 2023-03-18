#ifndef BOXCOLLIDER_COMPONENT_H
#define BOXCOLLIDER_COMPONENT_H

#include <glm.hpp>
#include "utils/constants.hpp"

struct BoxColliderComponent {
  glm::vec2 size;
  glm::vec2 offset;

explicit BoxColliderComponent(glm::vec2 size = constants::Defaults::BoxCollider::SIZE, //NOLINT
                              glm::vec2 offset = constants::Defaults::BoxCollider::OFFSET)
  : size(size), offset(offset){};
};



#endif
