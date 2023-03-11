#ifndef HEALTHBAR_COMPONENT_H
#define HEALTHBAR_COMPONENT_H

#include <glm.hpp>
#include "utils/constants.hpp"

struct HealthBarComponent {
  glm::vec2 size; // Width Height Dimensions
  glm::vec2 offset; // XY Position in pixels
  int32_t   fontSize; // in pixel
  std::string fontKey; // key in the asset store 
  glm::vec2 textBorder; // in pixels
  explicit HealthBarComponent(
                              glm::vec2 size = { //NOLINT
                              constants::Defaults::HealthBar::DEFAULT_WIDTH,
                              constants::Defaults::HealthBar::DEFAULT_HEIGHT
                              },
                              glm::vec2 offset = {0, 0}, //NOLINT
                              int32_t fontSize = constants::Defaults::HealthBar::DEFAULT_FONT_SIZE,
                              std::string fontKey = constants::Defaults::HealthBar::DEFAULT_FONT,
                              glm::vec2 textBorder = {0, 10} 

      ) :
    size(size),
    offset(offset),
    fontSize(fontSize),
    fontKey(std::move(fontKey)),
        textBorder(textBorder) {}
};


#endif
