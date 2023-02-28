#ifndef SPRITE_COMPONENT_HH
#define SPRITE_COMPONENT_HH
#include <glm.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <SDL2//SDL.h>


struct SpriteComponent {
  std::string key;
  glm::vec2 dimensions = glm::vec2(0,0);
  SDL_Rect crop = {0, 0, 0, 0};
  uint32_t zPosition = 0;

  SpriteComponent() = default; // resizing ComponentPools will call the default constructor

  SpriteComponent(std::string key, glm::vec2 spriteDimensions, SDL_Rect rect, uint32_t zPosition = 0)
      : key(std::move(key)), dimensions(spriteDimensions), crop(rect), zPosition(zPosition) {}

  static auto makeCrop(int32_t x, int32_t y, int32_t w, int32_t h){//NOLINT: short args
    return SDL_Rect {x, y, w, h};
  }
};
#endif


// give sprites layers
// layers can have name
// group sprite components by layers
// // or groupe gameObjects by layer?
