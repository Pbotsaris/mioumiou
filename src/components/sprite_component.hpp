#ifndef SPRITE_COMPONENT_HH
#define SPRITE_COMPONENT_HH
#include <SDL2/SDL.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <utility>

struct SpriteComponent {
  enum Orientation {Up, Right, Down, Left};

  std::string key;
  glm::vec2   size; // Pixels
  int32_t     zPosition; // xIndex
  bool        isSheet; // if sprite is a spritesheet
  bool        flippable;
  bool        isFixed; 
  SDL_Rect    crop; // pixels
  SDL_RendererFlip flip;
  Orientation orientation; // enum

  explicit SpriteComponent(
      std::string key            = "",
      glm::vec2 spriteDimensions = {0, 0},
      int32_t zPosition          = 0,
      bool isSheet               = false,
      bool flippable             = true,
      bool isFixed               = false,
      SDL_Rect rect              = {0, 0, 0, 0},
      SDL_RendererFlip flip      = SDL_FLIP_NONE,
      Orientation orientation    = Right
      )
      : key(std::move(key)),
        size(spriteDimensions),
        zPosition(zPosition),
        isSheet(isSheet),
        flippable(flippable),
        isFixed(isFixed),
        crop(rect),
        flip(flip),
        orientation(orientation)
  {

    if (crop.w == 0) {
      crop.w = size.x; // NOLINT
    }

    if (crop.h == 0) {
      crop.h = size.y; // NOLINT
    }
  }

  [[nodiscard]] auto getWidth() const -> int32_t {
    return static_cast<int32_t>(size.x); // NOLINT
  }

  [[nodiscard]] auto getHeight() const -> int32_t {
    return static_cast<int32_t>(size.y); // NOLINT
  }

  static auto makeCrop(int32_t x, int32_t y, int32_t w, int32_t h) { // NOLINT: short args
    return SDL_Rect{x, y, w, h};
  }
};
#endif
