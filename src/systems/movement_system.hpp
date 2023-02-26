#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H
#include "ecs/system.hpp"
#include "components/transform_component.hpp"
#include "components/rigid_body_component.hpp"
#include <spdlog/spdlog.h>

class  MovementSystem : public System {

  public:
  MovementSystem(){
  /* Register components as the required by this system */
  requiredComponent<TransformComponent>();
  requiredComponent<RigidBodyComponent>();
}

void update(double deltatime){

  for(auto &gameObject : gameObjects()){
    /* reference as we will modified transform */
    auto &transform = gameObject.getComponent<TransformComponent>();
    const auto rigidBody = gameObject.getComponent<RigidBodyComponent>();
    transform.position += (rigidBody.velocity * glm::vec2(deltatime, deltatime));
  }
}

};

#endif

