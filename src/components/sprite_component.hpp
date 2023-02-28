#ifndef SPRITE_COMPONENT_HH
#define SPRITE_COMPONENT_HH
#include <SDL2/SDL.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <utility>

struct SpriteComponent {
  std::string key;
  glm::vec2   dimensions;
  SDL_Rect    crop;
  uint32_t    zPosition;

  explicit SpriteComponent(std::string key,
                           glm::vec2   spriteDimensions = {0, 0},
                           SDL_Rect    rect = {0, 0, 0, 0},
                           uint32_t    zPosition = 0)
      : key(std::move(key)),
      dimensions(spriteDimensions),
      crop(rect),
      zPosition(zPosition) {}

  static auto makeCrop(int32_t x, int32_t y, int32_t w, int32_t h) {  // NOLINT: short args
    return SDL_Rect{x, y, w, h};
  }
};
#endif

