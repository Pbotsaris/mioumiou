#include "render_system.hpp"
#include <algorithm>

// Render system is responsible for sorting on x:
auto RenderSystem::sortedGameObjects() const -> std::vector<GameObject>{
  auto objs = gameObjects();
  std::sort(objs.begin(), objs.end(), [](const GameObject &left, const GameObject &right){
      const auto  leftSprite = left.getComponent<SpriteComponent>();
      const auto  rightSprite = right.getComponent<SpriteComponent>();
      return leftSprite.zPosition < rightSprite.zPosition;
      });

  return objs;
}


auto RenderSystem::name() const -> std::string {
  return "RenderSystem";
}
