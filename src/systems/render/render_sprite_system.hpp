#ifndef RENDER_SPRITE_SYSTEM_H
#define RENDER_SPRITE_SYSTEM_H
#include <memory>
#include <spdlog/spdlog.h>

#include "components/sprite_component.hpp"
#include "components/transform_component.hpp"
#include "components/projectile_component.hpp"
#include "ecs/system.hpp"
#include "game/asset_store.hpp"
#include "game/renderer.hpp"
#include "game/camera.hpp"

class RenderSpriteSystem : public System { // NOLINT

public:
  RenderSpriteSystem() {
    requiredComponent<TransformComponent>();
    requiredComponent<SpriteComponent>();
  }

  void update(std::unique_ptr<Renderer> &renderer,
      Camera &camera,
      std::unique_ptr<AssetStore> &store
      ) {

    for (auto &gameObject : sortedGameObjects()) {
      const auto transform = gameObject.getComponent<TransformComponent>();
      const auto sprite    = gameObject.getComponent<SpriteComponent>();

      /* culling Sprites. Ignored fixed position */
      if(!sprite.isFixed && isSpriteOutsideCameraView(transform, sprite, camera)){
        continue;
      }

      const glm::vec2 dimension  = sprite.size * transform.scale;
      SDL_Texture *tex           = store->getTexture(sprite.key);

      if (tex == nullptr) {
        spdlog::warn("Failed to render GameObject id '{}' sprite path: '{}'. " "Texture does not exist.", gameObject.id(), sprite.key);
        return;
      }

      /* NOTE : must render objects relative to camera position 
       *        if fixed, ignore relative camera position
       * */

          uint32_t offsetX = sprite.isFixed ? 0 : camera.x();
          uint32_t offsetY = sprite.isFixed ? 0 : camera.y();

         SDL_Rect destDimensions ={
             .x = static_cast<int>( transform.position.x - offsetX), // NOLINT: member of unions
             .y = static_cast<int>( transform.position.y - offsetY), // NOLINT: member of unions
             .w = static_cast<int>(dimension.x), // NOLINT
             .h = static_cast<int>(dimension.y), // NOLINT
          };

      renderer->drawImage(tex, ImageDimensions(&sprite.crop, &destDimensions, transform.rotation, sprite.flip));
    }
  }

  [[nodiscard]] auto name() const -> std::string override;
  [[nodiscard]] static auto isOutsideCameraView(const TransformComponent &transform, const Camera &camera) -> bool;

private:
  [[nodiscard]] auto sortedGameObjects() const -> std::vector<GameObject>;
  [[nodiscard]] static auto isSpriteOutsideCameraView(const TransformComponent &transform, const SpriteComponent &sprite, const Camera &camera) -> bool;

};

#endif
