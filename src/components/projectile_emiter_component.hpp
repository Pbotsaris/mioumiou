#ifndef PROJECTILE_EMITER_COMPONENT_H
#define PROJECTILE_EMITER_COMPONENT_H

#include <glm.hpp>
#include <string>
#include <SDL2/SDL.h>

struct ProjectileEmiterComponent {
  std::string spriteKey; 
  glm::vec2 size; // pixels
  glm::vec2 velocity; // pixel per sec
  uint32_t frequency; // millis
  uint32_t longevity; // millis
  int32_t percentDamage;
  glm::vec2 offset; // pixels
  uint32_t lastEmitTime; // millis

  explicit ProjectileEmiterComponent(
        std::string   spriteKey     = "",
        glm::vec2     dimension     = {0, 0}, //NOLINT
        glm::vec2     velocity      = {0, 0},
        uint32_t      frequency     = 500, //NOLINT
        uint32_t      longevity     = 500, //NOLINT
        int32_t       percentDamage = 10, //NOLINT
        glm::vec2     offset        = {0, 0}
        )
      : spriteKey(std::move(spriteKey)),
        size(dimension),
        velocity(velocity),
        frequency(frequency),
        longevity(longevity),
        percentDamage(percentDamage),
        offset(offset),
        lastEmitTime(SDL_GetTicks())
        {}
};

#endif
