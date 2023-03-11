#include "render_debug_system.hpp"

auto RenderDebugSystem::name() const -> std::string { return "RenderDebugSystem"; }

void RenderDebugSystem::onCollision(CollisionEvent &event){
  event.a().getComponent<DebugComponent>().collision.hasCollided = true;
  event.b().getComponent<DebugComponent>().collision.hasCollided = true;
}
