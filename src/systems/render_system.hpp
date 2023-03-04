#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include <memory>
#include <spdlog/spdlog.h>

#include "components/sprite_component.hpp"
#include "components/transform_component.hpp"
#include "ecs/system.hpp"
#include "game/asset_store.hpp"
#include "game/renderer.hpp"

class RenderSystem : public System { // NOLINT

public:
  RenderSystem() {
    requiredComponent<TransformComponent>();
    requiredComponent<SpriteComponent>();
  }

  void update(std::unique_ptr<Renderer> &renderer, std::unique_ptr<AssetStore> &store) {

    for (auto &gameObject : sortedGameObjects()) {

      const auto transform       = gameObject.getComponent<TransformComponent>();
      const auto sprite          = gameObject.getComponent<SpriteComponent>();
      const glm::vec2 dimension  = sprite.dimensions * transform.scale;
      SDL_Texture *tex           = store->getTexture(sprite.key);

      if (tex == nullptr) {
        spdlog::warn("Failed to render GameObject id '{}' sprite path: '{}'. " "Texture does not exist.", gameObject.id(), sprite.key);
        return;
      }

         SDL_Rect destDimensions ={
             .x = static_cast<int>( transform.position.x), // NOLINT: member of unions
             .y = static_cast<int>( transform.position.y), // NOLINT: member of unions
             .w = static_cast<int>(dimension.x), // NOLINT
             .h = static_cast<int>(dimension.y), // NOLINT
          };

      renderer->drawImage(tex, ImageDimensions(&sprite.crop, &destDimensions, transform.rotation));
    }
  }

  [[nodiscard]] auto name() const -> std::string override;

private:
  [[nodiscard]] auto sortedGameObjects() const -> std::vector<GameObject> ;
};

#endif
