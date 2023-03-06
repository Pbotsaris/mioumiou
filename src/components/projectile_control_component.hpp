#ifndef PROJECTILE_CONTROL_COMPONENT_H
#define PROJECTILE_CONTROL_COMPONENT_H

#include <SDL2/SDL.h>
#include <glm.hpp>

struct ProjectileControlComponent {
  SDL_Keycode keycode;

  explicit ProjectileControlComponent(
      SDL_Keycode keycode = SDLK_SPACE
      ) : keycode(keycode) {};
};

#endif
