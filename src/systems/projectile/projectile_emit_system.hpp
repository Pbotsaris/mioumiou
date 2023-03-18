#ifndef PROJECT_EMIT_SYSTEM_H
#define PROJECT_EMIT_SYSTEM_H

#include <SDL2/SDL.h>
#include <glm.hpp>
#include <memory>

#include "ecs/world_manager.hpp"
#include "components/all.hpp"
#include "ecs/system.hpp"
#include "event_system/event_bus.hpp"
#include "events/keypress_event.hpp"
#include "utils/configurables.hpp"

class ProjectileEmitSystem : public System {
public:
  ProjectileEmitSystem() {
    requiredComponent<ProjectileEmitterComponent>();
    requiredComponent<TransformComponent>();
  }


  void update() {
    for (auto &gameObject : gameObjects()) {

      auto &projectileEmiter = gameObject.getComponent<ProjectileEmitterComponent>();

      if(!isReadyToEmit(projectileEmiter)){
        continue;
      }

      emitProjectile(gameObject, projectileEmiter);
      projectileEmiter.lastEmitTime = SDL_GetTicks();
    }
  }


static void emitProjectile(GameObject &gameObject, const ProjectileEmitterComponent &projectileEmiter){

      const auto transform = gameObject.getComponent<TransformComponent>();

      /* can add an offset to initial position */
      glm::vec2 pos = transform.position + projectileEmiter.offset;
      auto velocity = projectileEmiter.velocity;

       /* in case projectileEmitter does not have sprite, the projectile will have the z pos just
        *  above the map level. Otherwise projectile takes the z position of the emitter sprite.
        */

       int32_t zPos = constants::Position::Z_NOTMAP; 

        /* in the middle for sprites, please */
      if(gameObject.hasComponent<SpriteComponent>()){
         const auto sprite = gameObject.getComponent<SpriteComponent>();
         pos += (sprite.size * transform.scale) / glm::vec2(2, 2);
         zPos = sprite.zPosition;
         // keyboard controlled objects will change emission direction
         if(gameObject.hasComponent<KeyboardControlComponent>()){
           // TODO: If KeyboardControlComponent Strategy is rotation we need to update this logic
          velocity = projectileEmiter.velocity * getEmissionDirection(sprite.orientation);
          pos += getEmissionDirection(sprite.orientation) * glm::vec2(8, 8); //NOLINT
        }
      }

      // projectile velocity always relative to object velocity
     if(gameObject.hasComponent<RigidBodyComponent>()){
         const auto rigidBody = gameObject.getComponent<RigidBodyComponent>();
         velocity += rigidBody.velocity;
      }

      /* use the reference to the world manager within every gameObject */
      auto projectile = gameObject.worldManager()->createGameObject();
      projectile.addComponent<TransformComponent>(pos, glm::vec2(1, 1));
      projectile.addComponent<RigidBodyComponent>(velocity);
      projectile.addComponent<SpriteComponent>(
          projectileEmiter.spriteKey,
          projectileEmiter.size,
          zPos,
          false, // sheet
          false, // flippable
          false, // fixed,
          SpriteComponent::makeCrop(0, 0,
            static_cast<int32_t>(projectileEmiter.size.x) , // NOLINT
            static_cast<int32_t>(projectileEmiter.size.y) // NOLINT
            )
          );
      projectile.addComponent<BoxColliderComponent>(projectileEmiter.size);
      projectile.toGroup(configurables::Groups::PROJECTILES);

     // projectile joins the same alliances as emiter
     for(const auto &alliance : gameObject.alliances()){
       projectile.joinAlliance(alliance);
     }
    
    projectile.addComponent<ProjectileComponent>(projectileEmiter.percentDamage, projectileEmiter.longevity);
}


  [[nodiscard]] auto name() const -> std::string override;

private:
static auto isReadyToEmit(ProjectileEmitterComponent &projectileEmiter) -> bool;
static auto getEmissionDirection(SpriteComponent::Orientation orientation) -> glm::vec2;

};

#endif
