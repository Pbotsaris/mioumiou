#ifndef RENDER_TEXT_SYSTEM_H
#define RENDER_TEXT_SYSTEM_H

#include <memory>
#include <spdlog/spdlog.h>

#include "ecs/system.hpp"
#include "game/asset_store.hpp"
#include "game/renderer.hpp"
#include "game/camera.hpp"
#include "components/text_component.hpp"
#include "components/camera_follower_component.hpp"


class RenderTextSystem : public System {

  public:
    RenderTextSystem(){
      requiredComponent<TextComponent>();
    }

    void update(std::unique_ptr<Renderer> &renderer, std::unique_ptr<AssetStore> &store, Camera &camera){
      for(auto &gameObject : gameObjects()){

        const auto text = gameObject.getComponent<TextComponent>();
        TTF_Font *font = store->getFont(text.assetKey, text.fontSize);


        SDL_Texture *texture = renderer->createTextureFromSurface(
            TTF_RenderText_Blended(font, text.content.c_str(), text.color));


        renderer->drawText(texture, {
            .x = static_cast<int32_t>(text.position.x) - (text.isFixed ?  0 : camera.x()), //NOLINT
            .y = static_cast<int32_t>(text.position.y) - (text.isFixed ? 0 : camera.y()) //NOLINT
            });
      }
    }

  [[nodiscard]] auto name() const -> std::string override;
};

#endif
