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
  explicit RenderSystem(std::shared_ptr<Renderer> &renderer)
      : m_renderer(renderer) {
    requiredComponent<TransformComponent>();
    requiredComponent<SpriteComponent>();
  }

  void update() {

    for (auto &gameObject : gameObjects()) {

      const auto transform = gameObject.getComponent<TransformComponent>();
      const auto sprite = gameObject.getComponent<SpriteComponent>();

      const glm::vec2 dimension = sprite.dimensions * transform.scale;

      bool valid = m_renderer->drawImage( sprite.path, {
              static_cast<int>(transform.position.x), // NOLINT: member of unions
              static_cast<int>(transform.position.y), // NOLINT: member of unions
              static_cast<int>(dimension.x), // NOLINT
              static_cast<int>(dimension.y), // NOLINT
          });

         spdlog::info("position -> x: {}, y: {}, size -> w: {}, h: 3", 
                     transform.position.x, transform.position.y,  dimension.x, dimension.y); //NOLINT

      if (!valid) {
         spdlog::warn("Failed to render GameObject id '{1}' sprite path: '{2}'",
                     gameObject.id(), sprite.path);
      }
    }
  }

private:
  std::shared_ptr<Renderer> m_renderer;
};

#endif
