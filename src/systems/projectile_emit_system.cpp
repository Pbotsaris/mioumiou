#include "systems/projectile_emit_system.hpp"


auto ProjectileEmitSystem::name() const -> std::string {
  return "ProjectileEmitSystem";
}


auto ProjectileEmitSystem::isReadyToEmit(ProjectileEmiterComponent &projectileEmiter) -> bool {
  if(projectileEmiter.frequency == 0){
    return false;
  }

 return ((SDL_GetTicks() - projectileEmiter.lastEmitTime) > projectileEmiter.frequency);
}

auto ProjectileEmitSystem::getEmissionDirection(const RigidBodyComponent &rigidBody)-> glm::vec2 {
  if(rigidBody.velocity.x > 0 && rigidBody.velocity.y > 0){ // NOLINT
    return {1, 1};
  }

  if(rigidBody.velocity.x < 0 && rigidBody.velocity.y < 0){ // NOLINT
    return {-1, -1};
  }

if(rigidBody.velocity.x > 0 && rigidBody.velocity.y < 0){ // NOLINT
    return { 1, -1};
  }

if(rigidBody.velocity.x < 0 && rigidBody.velocity.y > 0){ // NOLINT
    return { -1, 1};
  }

  return {1, 1};
}
