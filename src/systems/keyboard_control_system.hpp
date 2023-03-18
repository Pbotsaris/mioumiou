#ifndef KEYBOARD_CONTROL_SYSTEM_H
#define KEYBOARD_CONTROL_SYSTEM_H

#include <memory>
#include <spdlog/spdlog.h>
#include <cmath>

#include "utils/constants.hpp"
#include "ecs/system.hpp"
#include "components/all.hpp"
#include "event_system/event_bus.hpp"
#include "events/keypress_event.hpp"
#include "projectile/projectile_emit_system.hpp"

class KeyboardControlSystem : public System {

  public:
  KeyboardControlSystem(){
    requiredComponent<RigidBodyComponent>();
    requiredComponent<KeyboardControlComponent>();
    requiredComponent<SpriteComponent>();
    requiredComponent<TransformComponent>();
  }

  void addEventListeners(std::unique_ptr<EventBus> &eventBus){
    eventBus->addEventListener<KeyboardControlSystem, KeyDownEvent>(this, &KeyboardControlSystem::onKeyDown);
  }

  [[nodiscard]] auto name() const -> std::string override;

  private:
  void onKeyDown(KeyDownEvent &event){ //NOLINT
  
      for(auto &gameObject : gameObjects()) {
  
        const auto keyboardControl = gameObject.getComponent<KeyboardControlComponent>();
        auto &sprite               = gameObject.getComponent<SpriteComponent>();
        auto &rigidBody            = gameObject.getComponent<RigidBodyComponent>();
  
        if(!sprite.isSheet){
          spdlog::warn("GameObject id {} cannot be controlled by keyboard as it requires a spritesheet.");
          continue;
        }

        switch(event.key()){
          case SDLK_UP:
            rigidBody.velocity = keyboardControl.velocity.up;
            sprite.crop.y = sprite.getHeight() * constants::SpriteSheet::LOOK_UP;
            sprite.orientation = SpriteComponent::Up;
            break;
  
          case SDLK_RIGHT:
            rigidBody.velocity = keyboardControl.velocity.right;
            sprite.crop.y = sprite.getHeight() * constants::SpriteSheet::LOOK_RIGHT; 
            sprite.orientation = SpriteComponent::Right;

            break;
  
          case SDLK_DOWN:
            rigidBody.velocity = keyboardControl.velocity.down;
            sprite.crop.y = sprite.getHeight() * constants::SpriteSheet::LOOK_DOWN;
            sprite.orientation = SpriteComponent::Down;
            break;
  
          case SDLK_LEFT:
            rigidBody.velocity = keyboardControl.velocity.left;
            sprite.crop.y = sprite.getHeight() * constants::SpriteSheet::LOOK_LEFT;
            sprite.orientation = SpriteComponent::Left;
            break;
        };
    };
  }
};

#endif

