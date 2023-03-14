#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H
#include <spdlog/spdlog.h>
#include <memory>

#include "ecs/system.hpp"
#include "components/transform_component.hpp"
#include "components/rigid_body_component.hpp"
#include "components/sprite_component.hpp"
#include "utils/configurables.hpp"
#include "event_system/event_bus.hpp"
#include "events/collision_event.hpp"

class  MovementSystem : public System { //NOLINT

  enum Direction { None, Vertical, Horizontal, Both };

  public:
  MovementSystem(){
  /* Register components as the required by this system */
  requiredComponent<TransformComponent>();
  requiredComponent<RigidBodyComponent>();
}

void addEventListeners(std::unique_ptr<EventBus> &eventBus){
 eventBus->addEventListener<MovementSystem, CollisionEvent>(this, &MovementSystem::onCollision);
};

void update(double deltatime){

  for(auto &gameObject : gameObjects()){
    /* reference as we will modified transform */
    auto &transform = gameObject.getComponent<TransformComponent>();
    const auto rigidBody = gameObject.getComponent<RigidBodyComponent>();
    transform.position += (rigidBody.velocity * glm::vec2(deltatime, deltatime));

    /* checking map bounds  */
    bool isOffMap = (isOutBoundsX(transform) || isOutBoundsY(transform)); 

    if(gameObject.hasTag(constants::Tags::MAIN_PLAYER)){
      limitBounds(transform);
     continue;
    }

    if(isOffMap && gameObject.hasTag(constants::Tags::MAIN_PLAYER)){
      gameObject.remove();
    }
  }
}
  [[nodiscard]] auto name() const -> std::string override;

  private: 
  void onCollision(CollisionEvent &event);
  static void onEnemyHitsObstacle(GameObject bounderObject, GameObject obstacle);
   
  /* Helpers */
  static void handleSpriteSheetFlip(SpriteComponent &sprite, Direction direction);
  static void doSheetVertical(SpriteComponent &sprite);
  static void doSheetHorizontal(SpriteComponent &sprite);

  static auto isOutBoundsX(const TransformComponent &transform) -> bool;
  static auto isOutBoundsY(const TransformComponent &transform) -> bool;
  static void limitBounds(TransformComponent &transform);
};

#endif

