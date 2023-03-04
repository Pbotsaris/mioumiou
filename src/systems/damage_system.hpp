#ifndef DAMAGE_SYSTEM_H
#define DAMAGE_SYSTEM_H

#include "ecs/system.hpp"
#include "components/all.hpp"
#include "event_system/event_bus.hpp"
#include "events/collision_event.hpp"
#include <functional>

class DamageSystem : public System { //NOLINT
  public:

    DamageSystem(){
      requiredComponent<BoxColliderComponent>();
    }

    void addEventListeners(std::unique_ptr<EventBus> &eventBus) {
      eventBus->addEventListner<DamageSystem, CollisionEvent>(this, &DamageSystem::onCollision);
    };

    void update(){
      //TODO...
    }

  [[nodiscard]] auto name() const -> std::string override;

  private:
    /* Event callbacks */
    void onCollision(CollisionEvent &event);

};

#endif
