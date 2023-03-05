#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include <memory>

#include "ecs/system.hpp"
#include "components/all.hpp"
#include "event_system/event_bus.hpp"
#include "events/keypress_event.hpp"

class KeyboardMovementSystem : public System {

  public:
  KeyboardMovementSystem(){
    requiredComponent<TransformComponent>();
    requiredComponent<RigidBodyComponent>();
  }

  void addEventListeners(std::unique_ptr<EventBus> &eventBus){
    eventBus->addEventListner<KeyboardMovementSystem, KeyPressEvent>(this, &KeyboardMovementSystem::onKeyPressed);
  }

  [[nodiscard]] auto name() const -> std::string override;

  private:
    void onKeyPressed(KeyPressEvent &event);

};



#endif

