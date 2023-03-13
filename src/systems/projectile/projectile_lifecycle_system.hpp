#ifndef PROJECTILE_LIFECYCLE_SYSTEM_H
#define PROJECTILE_LIFECYCLE_SYSTEM_H
#include "ecs/system.hpp"

#include "components/projectile_component.hpp"
#include <SDL2/SDL.h>

class ProjectileLifeCycleSystem : public System {
public:
  ProjectileLifeCycleSystem() { requiredComponent<ProjectileComponent>(); }

  void update() {
    for (auto &gameObject : gameObjects()) {
      auto projectile = gameObject.getComponent<ProjectileComponent>();

      if (SDL_GetTicks() - projectile.startTime > projectile.duration ) {
        gameObject.remove();
      }
    }
  }

  [[nodiscard]] auto name() const -> std::string override;
};

#endif
