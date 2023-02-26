#ifndef SPRITE_COMPONENT_HH
#define SPRITE_COMPONENT_HH
#include <glm.hpp>
#include <iostream>
#include <string>
#include <utility>

struct SpriteComponent {
  std::string path;
  glm::vec2 dimensions = glm::vec2(0,0);

  SpriteComponent() = default; // resizing ComponentPools will call the default constructor

  SpriteComponent(std::string spritePath,
                  glm::vec2 spriteDimensions)
      : path(std::move(spritePath)), dimensions(spriteDimensions) {}
};
#endif
