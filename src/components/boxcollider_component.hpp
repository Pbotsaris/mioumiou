#ifndef BOXCOLLIDER_COMPONENT_H
#define BOXCOLLIDER_COMPONENT_H

#include <glm.hpp>

struct BoxColliderComponent {
  glm::vec2 size;
  glm::vec2 offset;

explicit BoxColliderComponent(glm::vec2 size = {0, 0}, //NOLINT
                              glm::vec2 offset = {0, 0}) //NOLINT
  : size(size), offset(offset){};
};



#endif
