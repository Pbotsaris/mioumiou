#ifndef SPRITE_COMPONENT_HH
#define SPRITE_COMPONENT_HH
#include <SDL2/SDL.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <utility>

struct SpriteComponent {
  std::string key;
  glm::vec2 dimensions;
  int32_t zPosition;
  bool isFixed;
  SDL_Rect crop;

  explicit SpriteComponent(
      std::string key            = "",
      glm::vec2 spriteDimensions = {0, 0},
      int32_t zPosition          = 0,
      bool isFixed               = false,
      SDL_Rect rect              = {0, 0, 0, 0})
      : key(std::move(key)),
        dimensions(spriteDimensions),
        zPosition(zPosition),
        isFixed(isFixed),
        crop(rect) {

    if (crop.w == 0) {
      crop.w = dimensions.x; // NOLINT
    }

    if (crop.h == 0) {
      crop.h = dimensions.y; // NOLINT
    }
  }

  [[nodiscard]] auto getWidth() const -> int32_t {
    return static_cast<int32_t>(dimensions.x); // NOLINT
  }

  [[nodiscard]] auto getHeight() const -> int32_t {
    return static_cast<int32_t>(dimensions.y); // NOLINT
  }

  static auto makeCrop(int32_t x, int32_t y, int32_t w, int32_t h) { // NOLINT: short args
    return SDL_Rect{x, y, w, h};
  }
};
#endif
