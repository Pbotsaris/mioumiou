#ifndef PROJECT_EMIT_SYSTEM_H
#define PROJECT_EMIT_SYSTEM_H

#include "ecs/world_manager.hpp"
#include <SDL2/SDL.h>
#include <glm.hpp>
#include <memory>

#include "components/all.hpp"
#include "ecs/system.hpp"

class ProjectileEmitSystem : public System {
public:
  ProjectileEmitSystem() {
    requiredComponent<ProjectileEmiterComponent>();
    requiredComponent<TransformComponent>();
  }

  void update(std::unique_ptr<WorldManager> &worldManager) {

    for (auto &gameObject : gameObjects()) {

      auto &projectileEmiter =
          gameObject.getComponent<ProjectileEmiterComponent>();
      const auto transform = gameObject.getComponent<TransformComponent>();

      if (SDL_GetTicks() - projectileEmiter.lastEmitTime >
          projectileEmiter.frequency) {

        glm::vec2 pos = transform.position;

        /* if object is a sprite center the projectile in the middle of the sprite */
        if (gameObject.hasComponent<SpriteComponent>()) {
          const auto sprite = gameObject.getComponent<SpriteComponent>();
          pos += (sprite.dimensions * transform.scale) / glm::vec2(2, 2);
        }

        GameObject projectile = worldManager->createGameObject();
        projectile.addComponent<TransformComponent>(pos, glm::vec2(1, 1));
        projectile.addComponent<RigidBodyComponent>(projectileEmiter.velocity);
        projectile.addComponent<SpriteComponent>(projectileEmiter.spriteKey,
                                                 projectileEmiter.dimension, 4);
        projectile.addComponent<BoxColliderComponent>(
            projectileEmiter.dimension);

        projectileEmiter.lastEmitTime = SDL_GetTicks();
      }
    }
  }

  [[nodiscard]] auto name() const -> std::string override;
};

#endif
