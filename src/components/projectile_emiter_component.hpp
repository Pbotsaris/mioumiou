#ifndef PROJECTILE_EMITER_COMPONENT_H
#define PROJECTILE_EMITER_COMPONENT_H

#include <glm.hpp>
#include <string>
#include <SDL2/SDL.h>
#include "utils/constants.hpp"

struct ProjectileEmitterComponent {
  std::string spriteKey; 
  glm::vec2 size; // pixels
  glm::vec2 velocity; // pixel per sec
  uint32_t frequency; // millis
  uint32_t longevity; // millis
  int32_t percentDamage;
  glm::vec2 offset; // pixels
  uint32_t lastEmitTime; // millis

  explicit ProjectileEmitterComponent(
        std::string   spriteKey     = "",
        glm::vec2     size          = constants::Defaults::ProjectileEmitter::SIZE, // NOLINT
        glm::vec2     velocity      = constants::Defaults::ProjectileEmitter::VELOCITY,
        uint32_t      frequency     = constants::Defaults::ProjectileEmitter::FREQUENCY, // NOLINT
        uint32_t      longevity     = constants::Defaults::ProjectileEmitter::LONGEVITY,
        int32_t       percentDamage = constants::Defaults::ProjectileEmitter::PERCENT_DAMAGE,
        glm::vec2     offset        = constants::Defaults::ProjectileEmitter::OFFSET
        )
      : spriteKey(std::move(spriteKey)),
        size(size),
        velocity(velocity),
        frequency(frequency),
        longevity(longevity),
        percentDamage(percentDamage),
        offset(offset),
        lastEmitTime(SDL_GetTicks())
        {}
};

#endif
