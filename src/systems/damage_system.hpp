#ifndef DAMAGE_SYSTEMS_H
#define DAMAGE_SYSTEMS_H

#include <spdlog/spdlog.h>

#include "components/all.hpp"
#include "ecs/system.hpp"
#include "event_system/event_bus.hpp"
#include "events/collision_event.hpp"
#include "components/health_component.hpp"

class DamageSystem : public System { // NOLINT
public:
  DamageSystem() { requiredComponent<BoxColliderComponent>(); }

  void addEventListeners(std::unique_ptr<EventBus> &eventBus) {
    eventBus->addEventListener<DamageSystem, CollisionEvent>(
        this, &DamageSystem::onCollision);
  };

  void update() {
    // TODO...
  }

  [[nodiscard]] auto name() const -> std::string override;

  /* Event callbacks */
  void onCollision(CollisionEvent &event);
private:
  void damageOnPlayer(GameObject player, GameObject projectile); // NOLINT
};

#endif
