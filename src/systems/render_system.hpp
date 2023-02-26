#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include <memory>
#include <spdlog/spdlog.h>

#include "components/sprite_component.hpp"
#include "components/transform_component.hpp"
#include "ecs/system.hpp"
#include "game/renderer.hpp"

class RenderSystem : public System {

public:
   RenderSystem(){
    requiredComponent<TransformComponent>();
    requiredComponent<SpriteComponent>();
  }

  void update(std::unique_ptr<Renderer> &renderer) {

    for (auto &gameObject : gameObjects()) {

      const auto transform = gameObject.getComponent<TransformComponent>();
      const auto sprite = gameObject.getComponent<SpriteComponent>();

      const glm::vec2 dimension = sprite.dimensions * transform.scale;

      bool valid = renderer->drawImage( sprite.path, {
              static_cast<int>(transform.position.x), // NOLINT: member of unions
              static_cast<int>(transform.position.y), // NOLINT: member of unions
              static_cast<int>(dimension.x), // NOLINT
              static_cast<int>(dimension.y), // NOLINT
          });

      if (!valid) {
         spdlog::warn("Failed to render GameObject id '{1}' sprite path: '{2}'",
                     gameObject.id(), sprite.path);
      }
    }
  }

private:
};

#endif
