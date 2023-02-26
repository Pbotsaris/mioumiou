#ifndef SPRITE_COMPONENT_HH
#define SPRITE_COMPONENT_HH
#include <glm.hpp>
#include <iostream>
#include <string>
#include <utility>

struct SpriteComponent {
  std::string key;
  glm::vec2 dimensions = glm::vec2(0,0);

  SpriteComponent() = default; // resizing ComponentPools will call the default constructor

  SpriteComponent(std::string key,
                  glm::vec2 spriteDimensions)
      : key(std::move(key)), dimensions(spriteDimensions) {}
};
#endif
