#ifndef PROJECTILE_COMPONENT_H
#define PROJECTILE_COMPONENT_H

#include <cstdint>
#include <SDL2/SDL.h>

struct ProjectileComponent {
  std::int32_t percentDamage; // percentage
  std::uint32_t duration;      // millis
  std::uint32_t startTime;    // millies

  explicit ProjectileComponent(
     std::int32_t percentDamage = 0, // NOLINT
     std::uint32_t duration     = 0
      ) :
      percentDamage(percentDamage),
      duration(duration),
      startTime(SDL_GetTicks())
     {}
};

#endif
