#ifndef PROJECTILE_EMITER_COMPONENT_H
#define PROJECTILE_EMITER_COMPONENT_H

#include <glm.hpp>
#include <string>
#include <SDL2/SDL.h>

struct ProjectileEmiterComponent {
  std::string spriteKey;
  glm::vec2 dimension;
  glm::vec2 velocity;
  uint32_t frequency;
  uint32_t longevity;
  uint32_t percentDamge;
  bool isFriendly;
  uint32_t lastEmitTime;

  explicit ProjectileEmiterComponent(
                            std::string spriteKey = "",
                            glm::vec2 dimension = {0, 0}, //NOLINT
                            glm::vec2 velocity = {0, 0},
                            uint32_t frequency = 500, //NOLINT
                            uint32_t longevity = 500, //NOLINT
                            uint32_t percentDamage = 10, //NOLINT
                            bool isFriendly = false,
                            uint32_t lastEmitTime = 0
                             )
      : spriteKey(
        std::move(spriteKey)),
        dimension(dimension),
        velocity(velocity),
        frequency(frequency),
        longevity(longevity),
        percentDamge(percentDamage),
        isFriendly(isFriendly),
        lastEmitTime(lastEmitTime)
        {
          lastEmitTime = SDL_GetTicks();
        }
};

#endif
