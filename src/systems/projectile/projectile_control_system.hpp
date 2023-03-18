#ifndef PROJECTILE_CONTROL_SYSTEM
#define PROJECTILE_CONTROL_SYSTEM

#include <memory>
#include <glm.hpp>
#include <SDL2/SDL.h>

#include "projectile_emit_system.hpp"

class ProjectileControlSystem : public System {
  public:
  ProjectileControlSystem() {
    requiredComponent<ProjectileEmitterComponent>();
    requiredComponent<ProjectileControlComponent>();
    requiredComponent<TransformComponent>();

  }

  void addEventListeners(std::unique_ptr<EventBus> &eventBus){
    eventBus->addEventListener<ProjectileControlSystem, KeyDownEvent>(this, &ProjectileControlSystem::onKeyDown);
  }


  [[nodiscard]] auto name() const -> std::string override;

  private:
  void onKeyDown(KeyDownEvent &event){

    for(auto &gameObject : gameObjects()){
      const auto projectileControl = gameObject.getComponent<ProjectileControlComponent>();

      if(event.key() != projectileControl.keycode){
        continue;
      }

      const auto projectileEmiter = gameObject.getComponent<ProjectileEmitterComponent>();
      ProjectileEmitSystem::emitProjectile(gameObject, projectileEmiter);

    }
  }
};



#endif
