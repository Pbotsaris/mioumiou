#ifndef SPRITE_COMPONENT_HH
#define SPRITE_COMPONENT_HH
#include <SDL2/SDL.h>
#include <glm.hpp>
#include <iostream>
#include <string>
#include <utility>
#include "utils/constants.hpp"

struct SpriteComponent {
  enum Orientation {Up = 0, Right = 1, Down = 2, Left = 3};

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
      glm::vec2 size             = constants::Defaults::Sprite::SIZE,
      int32_t zPosition          = constants::Defaults::Sprite::Z_POSITION,
      bool isSheet               = constants::Defaults::Sprite::IS_SHEET,
      bool flippable             = constants::Defaults::Sprite::FLIPABLE,
      bool isFixed               = constants::Defaults::Sprite::IS_FIXED,
      SDL_Rect crop              = constants::Defaults::Sprite::CROP,
      SDL_RendererFlip flip      =constants::Defaults::Sprite::FLIP, 
      Orientation orientation    = static_cast<Orientation>(constants::Defaults::Sprite::ORIENTATION)
      )
      : key(std::move(key)),
        size(size),
        zPosition(zPosition),
        isSheet(isSheet),
        flippable(flippable),
        isFixed(isFixed),
        crop(crop),
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
