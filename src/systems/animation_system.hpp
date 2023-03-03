#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <SDL2/SDL.h>
#include <memory>
#include <spdlog/spdlog.h>

#include "components/animation_component.hpp"
#include "components/sprite_component.hpp"
#include "ecs/system.hpp"
#include "utils/constants.hpp"

class AnimationSystem : public System {
  public:
  AnimationSystem() {
    requiredComponent<SpriteComponent>();
    requiredComponent<AnimationComponent>();
  }

  //TODO: Improve using string/names to identify animation steps instead of numbers

  void update() {
    for (auto &gameObject : gameObjects()) {
      /* taking refs as we are modyfing components */
      auto &animation = gameObject.getComponent<AnimationComponent>();
      auto &sprite = gameObject.getComponent<SpriteComponent>();

      uint32_t delta_millis = SDL_GetTicks() - animation.startTime;
      
      /* frameRateSpeed determines now many frames per second the animation will update */
      uint32_t frame = (delta_millis * animation.frameRateSpeed) / constants::Time::MILLI_TOSEC_MULT;

      /* ensures frame is always within nbFrames */
      animation.currentFrame = frame % animation.nbFrames; 
      sprite.crop.x = static_cast<int32_t>(animation.currentFrame * sprite.getWidth());
    }
  }

  [[nodiscard]] auto name() const -> std::string override;
};



#endif
