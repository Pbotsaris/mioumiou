#ifndef HEALTHBAR_COMPONENT_H
#define HEALTHBAR_COMPONENT_H

#include "utils/constants.hpp"
#include <glm.hpp>

struct HealthBarComponent {
   enum BarAlign {
      Left,
      Right,
      Top,
      Bottom
    };

  glm::vec2 size;   // Width Height Dimensions
  glm::vec2 offset; // XY Position in pixels
  BarAlign  alignment;
                    
  explicit HealthBarComponent(
      glm::vec2 size = constants::Defaults::HealthBar::SIZE, // NOLINT
      glm::vec2 offset = {0, 0},                             // NOLINT
      BarAlign alignment = Bottom
      )
      : size(size), offset(offset), alignment(alignment) {}
};

#endif
