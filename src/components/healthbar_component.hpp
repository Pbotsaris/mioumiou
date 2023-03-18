#ifndef HEALTHBAR_COMPONENT_H
#define HEALTHBAR_COMPONENT_H

#include "utils/constants.hpp"
#include <spdlog/spdlog.h>
#include <glm.hpp>

struct HealthBarComponent {
   enum BarAlign {
      Top = 0,
      Right = 1,
      Bottom = 2,
      Left = 3
    };

  bool visible;
  glm::vec2 size;   // Width Height Dimensions
  glm::vec2 offset; // XY Position in pixels
  BarAlign  alignment;
                    
  explicit HealthBarComponent(
      bool visible = constants::Defaults::HealthBar::VISIBLE,
      glm::vec2 size = constants::Defaults::HealthBar::SIZE, // NOLINT
      glm::vec2 offset = constants::Defaults::HealthBar::OFFSET,                     
      BarAlign alignment = static_cast<BarAlign>(constants::Defaults::HealthBar::ALIGNMENT) // BOTTOM
      )
      : 

      visible(visible),
      size(size),
      offset(offset),
      alignment(static_cast<BarAlign>(alignment)) {}
};

#endif
