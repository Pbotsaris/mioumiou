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

auto RenderSpriteSystem::isOutsideCameraView(const TransformComponent &transform, const Camera &camera) -> bool{
  return transform.position.x < static_cast<float>(camera.x()) ||  //NOLINT
         transform.position.x > static_cast<float>(camera.x() + camera.width()) ||  //NOLINT
         transform.position.y < static_cast<float>(camera.y()) ||   //NOLINT
         transform.position.y > static_cast<float>(camera.y() + camera.height());   //NOLINT
}

auto RenderSpriteSystem::isSpriteOutsideCameraView(const TransformComponent &transform, const SpriteComponent &sprite, const Camera &camera) -> bool{
  return transform.position.x + (sprite.getWidth() * transform.scale.x)  < static_cast<float>(camera.x()) ||  //NOLINT
         transform.position.x  > static_cast<float>(camera.x() + camera.width()) ||  //NOLINT
         transform.position.y + (sprite.getHeight() * transform.scale.y)  < static_cast<float>(camera.y()) ||   //NOLINT
         transform.position.y  > static_cast<float>(camera.y() + camera.height());   //NOLINT
}
