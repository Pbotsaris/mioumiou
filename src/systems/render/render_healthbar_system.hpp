#ifndef RENDER_HEALTHBAR_SYSTEM_H
#define RENDER_HEALTHBAR_SYSTEM_H

#include <memory>

#include "components//sprite_component.hpp"
#include "components/health_component.hpp"
#include "components/healthbar_component.hpp"
#include "components/healthbar_text_component.hpp"
#include "components/transform_component.hpp"
#include "ecs/system.hpp"
#include "game/asset_store.hpp"
#include "game/camera.hpp"
#include "game/renderer.hpp"
#include "systems/render/render_sprite_system.hpp"

class RenderHealthBarSystem : public System {
public:
  RenderHealthBarSystem() {
    requiredComponent<HealthBarComponent>();
    requiredComponent<HealthComponent>();
    requiredComponent<TransformComponent>();
    requiredComponent<SpriteComponent>();
  }

  void update(std::unique_ptr<Renderer> &renderer,
              std::unique_ptr<AssetStore> &store, Camera &camera) {

    for (auto &gameObject : gameObjects()) {

      const auto transform = gameObject.getComponent<TransformComponent>();

      /* culling as main RenderSpriteSystem */
      if(RenderSpriteSystem::isOutsideCameraView(transform, camera)){
        continue;
      }

      const auto health    = gameObject.getComponent<HealthComponent>();
      const auto bar       = gameObject.getComponent<HealthBarComponent>();
      const auto sprite    = gameObject.getComponent<SpriteComponent>();

      SDL_Color color  = setColor(renderer, health);
      glm::vec2 barPos = alignBar(transform, sprite, bar, camera);

      SDL_Rect healthbar = {
          .x = static_cast<int32_t>(barPos.x),   // NOLINT
          .y = static_cast<int32_t>(barPos.y),   // NOLINT
          .w = static_cast<int32_t>(bar.size.x), // NOLINT
          .h = static_cast<int32_t>(bar.size.y), // NOLINT
      };

      renderer->drawRect(&healthbar);
      int32_t fillWidth = health.currentHealth * healthbar.w / health.maxHealth;

      SDL_Rect healthbarFill = {
          .x = static_cast<int32_t>(barPos.x),   // NOLINT
          .y = static_cast<int32_t>(barPos.y),   // NOLINT
          .w = fillWidth,                        // NOLINT
          .h = static_cast<int32_t>(bar.size.y), // NOLINT
      };

      renderer->fillRect(&healthbarFill);

      if (!gameObject.hasComponent<HealthBarTextComponent>()) {
        continue;
      }

      const auto barText = gameObject.getComponent<HealthBarTextComponent>();

      auto percentage = std::to_string((health.currentHealth * constants::Percentage::DIVISOR)
          / health.maxHealth) + "%";

      auto *font = store->getFont(barText.font, barText.fontSize);

      SDL_Texture *texture = renderer->createTextureFromSurface(
          TTF_RenderText_Blended(font, percentage.c_str(), color));

      glm::vec2 textPos = barPos + glm::vec2(0, bar.size.y) + glm::vec2(0, barText.margin); // NOLINT

      renderer->drawText(
          texture, {
              .x = static_cast<int32_t>(textPos.x), // NOLINT
              .y = static_cast<int32_t>(textPos.y), // NOLINT
          });
    }
  }

  [[nodiscard]] auto name() const -> std::string override;

private:
  static auto setColor(std::unique_ptr<Renderer> &renderer,
                       const HealthComponent &health) -> SDL_Color;

  static auto alignBar(const TransformComponent &transform, const SpriteComponent &sprite,
                      const HealthBarComponent &bar, const Camera &camera) -> glm::vec2;

  static auto alignBarText(const HealthBarComponent &bar,
                          const HealthBarTextComponent &barText, const glm::vec2 &barPos) -> glm::vec2;

};

#endif
