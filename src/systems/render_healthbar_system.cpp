#include "systems/render_healthbar_system.hpp"
#include "game/colors.hpp"

#include "utils/constants.hpp"
auto RenderHealthBarSystem::name() const-> std::string { return "RenderHealthBarSystem"; }

auto RenderHealthBarSystem::setColor(std::unique_ptr<Renderer> &renderer, const HealthComponent &health) -> SDL_Color{

  int32_t onethirds = health.maxHealth / 3; //NOLINT
  int32_t twothirds = onethirds * 2;
  if(health.currentHealth > twothirds){
    renderer->setDrawColor(Green());
   return Green().getColor();
  }

 if(health.currentHealth < twothirds && health.currentHealth > onethirds){
   renderer->setDrawColor(Yellow());
   return Yellow().getColor();
  }

 renderer->setDrawColor(Red());
 return Red().getColor();
}



