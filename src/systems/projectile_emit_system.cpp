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

auto ProjectileEmitSystem::getEmissionDirection(SpriteComponent::Orientation orientation)-> glm::vec2 {

  switch(orientation){

    case SpriteComponent::Up:
    return {0, -1};

    case SpriteComponent::Right:
    return {1, 0};

    case SpriteComponent::Down:
    return {0, 1};

    case SpriteComponent::Left:
    return {-1, 0};
  }
}
