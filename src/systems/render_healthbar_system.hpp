#ifndef RENDER_HEALTHBAR_SYSTEM_H
#define RENDER_HEALTHBAR_SYSTEM_H

#include <memory>

#include "ecs/system.hpp"
#include "game/renderer.hpp"
#include "game/asset_store.hpp"
#include "game/camera.hpp"
#include "components/healthbar_component.hpp"
#include "components/health_component.hpp"
#include "components/transform_component.hpp"
#include "components//sprite_component.hpp"

class RenderHealthBarSystem : public System {
 public:
   RenderHealthBarSystem(){
     requiredComponent<HealthBarComponent>();
     requiredComponent<HealthComponent>();
     requiredComponent<TransformComponent>();
  }

   void update(std::unique_ptr<Renderer> &renderer,
       std::unique_ptr<AssetStore> &store, Camera &camera){

     for(auto &gameObject : gameObjects()){

       const auto transform = gameObject.getComponent<TransformComponent>();
       const auto health    = gameObject.getComponent<HealthComponent>();
       const auto bar       = gameObject.getComponent<HealthBarComponent>();

       SDL_Color color = setColor(renderer, health);

       double objHeight =  0;
       double objWidth = 0;
       double leftMargin = 0;
    
       // handle sprite objects
       if(gameObject.hasComponent<SpriteComponent>()){
         const auto sprite = gameObject.getComponent<SpriteComponent>();
         objWidth = static_cast<double>(sprite.getWidth()) * transform.scale.x;   //NOLINT
         objHeight = static_cast<double>(sprite.getHeight()) * transform.scale.y; //NOLINT
         // center
         leftMargin = (objWidth- bar.size.x) / 2;  // NOLINT
       }

       int32_t barPosX = static_cast<int32_t>(transform.position.x + bar.offset.x + leftMargin) - camera.x(); //NOLINT
       int32_t barPosY = static_cast<int32_t>(transform.position.y + objHeight + bar.offset.y) - camera.y(); //NOLINT

       SDL_Rect healthbar = {
           .x = barPosX,
           .y = barPosY,
           .w = static_cast<int32_t>(bar.size.x), //NOLINT
           .h = static_cast<int32_t>(bar.size.y), //NOLINT
       };

       renderer->drawRect(&healthbar);
       int32_t fillWidth = health.currentHealth * healthbar.w / health.maxHealth;

       SDL_Rect healthbarFill = {
           .x = barPosX,
           .y = barPosY,
           .w = fillWidth, //NOLINT
           .h = static_cast<int32_t>(bar.size.y), //NOLINT
       };

       renderer->fillRect(&healthbarFill);

       auto percentage = std::to_string((health.currentHealth * constants::Percentage::DIVISOR)
            / health.maxHealth) + "%";

       
       auto *font =store->getFont(bar.fontKey);

        SDL_Texture *texture = renderer->createTextureFromSurface(
            TTF_RenderText_Blended(font, percentage.c_str(), color));

        renderer->drawText(texture, {
            .x = barPosX + static_cast<int32_t>(bar.textBorder.x), // NOLINT
            .y = barPosY + static_cast<int32_t>(bar.textBorder.y), // NOLINT
            });
     }
  }

   [[nodiscard]] auto name() const -> std::string override;

 private:
   static auto setColor(std::unique_ptr<Renderer> &renderer, const HealthComponent &health)-> SDL_Color;
};

#endif
