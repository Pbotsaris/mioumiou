#include "systems/damage_system.hpp"
#include <spdlog/spdlog.h>

auto DamageSystem::name()const->std::string { return "DamageSystem"; }

void DamageSystem::onCollision(CollisionEvent &event){
  spdlog::debug("damage system receive onCollision of '{}' and '{}'", event.a().id(), event.b().id());

  event.a().remove();
  event.b().remove();
}

