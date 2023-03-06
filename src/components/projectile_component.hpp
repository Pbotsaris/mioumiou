#ifndef PROJECTILE_COMPONENT_H
#define PROJECTILE_COMPONENT_H

#include <cstdint>
#include <SDL2/SDL.h>

struct ProjectileComponent {
  bool isFrieldy;
  std::uint32_t percentDamage; // percentage
  std::uint32_t duration;      // millis
  std::uint32_t startTime;

  explicit ProjectileComponent(
     bool         isFrieldy     = false,
     std::uint32_t percentDamage = 0, // NOLINT
     std::uint32_t duration      = 0
      )
    : isFrieldy(isFrieldy),
      percentDamage(percentDamage),
      duration(duration),
      startTime(SDL_GetTicks())
     {}
};

#endif
