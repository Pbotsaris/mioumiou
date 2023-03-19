#include "systems/damage_system.hpp"

#include "utils/constants.hpp"
#include "utils/configurables.hpp"

using namespace constants;
using namespace configurables;

auto DamageSystem::name()const->std::string { return "DamageSystem"; }

void DamageSystem::onCollision(CollisionEvent &event) { // NOLINT
                                                        // projectiles will damage non allied GameObjects
  spdlog::debug("damage system receive onCollision of '{}' and '{}'", event.a().id(), event.b().id());

  if(event.b().belongsTo(configurables::Groups::PROJECTILES) && !event.b().isAllied(event.a())){
    doProjectileDamage(event.a(), event.b());
  }
  if(event.a().belongsTo(configurables::Groups::PROJECTILES) && !event.a().isAllied(event.b())){
    doProjectileDamage(event.b(), event.a());
  }
}

void DamageSystem::doProjectileDamage(GameObject gameObject, GameObject projectile) { //NOLINT
                                                                                  
  /* object must have a health component */
  if(!gameObject.hasComponent<HealthComponent>()){
    return;
  };

  const auto projectileComponent = projectile.getComponent<ProjectileComponent>();

  auto &playerHealth = gameObject.getComponent<HealthComponent>();

  /* calculate percentage in players health */
  playerHealth.currentHealth -= (playerHealth.maxHealth * projectileComponent.percentDamage) / Percentage::DIVISOR;

  if (playerHealth.currentHealth <= 0) {
    gameObject.remove();
  };

  projectile.remove();
};
