#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H

#include <string>
#include <glm.hpp>
#include <SDL2/SDL.h>
#include "game/colors.hpp"

struct TextComponent {
  std::string content;
  std::string assetKey;
  glm::vec2 position;
  SDL_Color color;
  bool isFixed;
  

  explicit TextComponent(
      std::string content = "",
      std::string assetKey ="",
      glm::vec2 position = {0, 0},
      const Colors &color = Black(),
      bool isFixed = true
      ) :
        content(std::move(content)),
        assetKey(std::move(assetKey)),
        position(position),
        color(color.getColor()),
        isFixed(isFixed)
  {}
};

#endif
