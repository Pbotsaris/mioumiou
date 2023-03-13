#include "systems/render/render_sprite_system.hpp"
#include <algorithm>

// Render system is responsible for sorting on x:
auto RenderSpriteSystem::sortedGameObjects() const -> std::vector<GameObject>{
  auto objs = gameObjects();
  std::sort(objs.begin(), objs.end(), [](const GameObject &left, const GameObject &right){
      const auto  leftSprite = left.getComponent<SpriteComponent>();
      const auto  rightSprite = right.getComponent<SpriteComponent>();
      return leftSprite.zPosition < rightSprite.zPosition;
      });

  return objs;
}


auto RenderSpriteSystem::name() const -> std::string {
  return "RenderSystem";
}
