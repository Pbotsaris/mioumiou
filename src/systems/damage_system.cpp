#include "systems/damage_system.hpp"

#include "utils/constants.hpp"
#include "utils/configurables.hpp"

using namespace constants;
using namespace configurables;

auto DamageSystem::name()const->std::string { return "DamageSystem"; }

void DamageSystem::onCollision(CollisionEvent &event) { // NOLINT
  spdlog::debug("damage system receive onCollision of '{}' and '{}'", event.a().id(), event.b().id());


  // projectiles will damage non allied GameObjects
  if(event.b().belongsTo(Groups::PROJECTILES) && !event.b().isAllied(event.a())){
    damageOnPlayer(event.a(), event.b());
  }
  if(event.a().belongsTo(Groups::PROJECTILES) && !event.a().isAllied(event.b())){
    damageOnPlayer(event.b(), event.a());
  }
}

void DamageSystem::damageOnPlayer(GameObject player, GameObject projectile) { //NOLINT

  const auto projectileComponent = projectile.getComponent<ProjectileComponent>();
  auto &playerHealth = player.getComponent<HealthComponent>();

  /* calculate percentage in players health */
  playerHealth.currentHealth -= (playerHealth.maxHealth * projectileComponent.percentDamage) / Percentage::DIVISOR;

  if (playerHealth.currentHealth <= 0) {
    player.remove();
  };

  projectile.remove();
};


// 1. are you a projectile
// 2. are you in the same alliance
