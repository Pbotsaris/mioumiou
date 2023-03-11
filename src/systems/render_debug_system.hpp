#ifndef RENDER_DEBUG_SYSTEM_H
#define RENDER_DEBUG_SYSTEM_H

#include <SDL2/SDL.h>
#include <memory>

#include "components/all.hpp"
#include "ecs/system.hpp"
#include "game/colors.hpp"
#include "game/renderer.hpp"
#include "events/collision_event.hpp"
#include "event_system/event_bus.hpp"
#include "game/camera.hpp"

class RenderDebugSystem : public System { //NOLINT

public:
  RenderDebugSystem() {
    requiredComponent<DebugComponent>();
    requiredComponent<BoxColliderComponent>();
    requiredComponent<TransformComponent>();
  }

  void addEventListeners(std::unique_ptr<EventBus> &eventBus){
    eventBus->addEventListener<RenderDebugSystem, CollisionEvent>(this, &RenderDebugSystem::onCollision);
  }

  void update(std::unique_ptr<Renderer> &renderer, Camera &camera) {
    for (auto &gameObject : gameObjects()) {

      auto &debug = gameObject.getComponent<DebugComponent>();

      if (!debug.collision.isVisible) {
        continue;
      }

      const auto transform = gameObject.getComponent<TransformComponent>();
      const auto collider = gameObject.getComponent<BoxColliderComponent>();


      SDL_Rect debugBorder{
          .x = static_cast<int>(transform.position.x + collider.offset.x - camera.x()), // NOLINT
          .y = static_cast<int>(transform.position.y + collider.offset.y - camera.y()), // NOLINT
          .w = static_cast<int>(collider.size.x * transform.scale.x),      // NOLINT
          .h = static_cast<int>(collider.size.y * transform.scale.y),      // NOLINT
      };

      if (debug.collision.hasCollided) {
        renderer->setDrawColor(Red());
      } else {
        renderer->setDrawColor(Yellow());
      }

      renderer->drawRect(&debugBorder);

      debug.collision.hasCollided = false;
    }
  }

  [[nodiscard]] auto name() const -> std::string override;

private:
   void onCollision(CollisionEvent &event);
};

#endif
