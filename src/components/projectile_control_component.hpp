#ifndef PROJECTILE_CONTROL_COMPONENT_H
#define PROJECTILE_CONTROL_COMPONENT_H

#include <SDL2/SDL.h>
#include <glm.hpp>
#include "utils/constants.hpp"

struct ProjectileControlComponent {
  SDL_Keycode keycode;

  explicit ProjectileControlComponent(
      SDL_Keycode keycode = constants::Defaults::ProjectileEmitter::CONTROL_KEY
      ) : keycode(keycode) {};
};

#endif
