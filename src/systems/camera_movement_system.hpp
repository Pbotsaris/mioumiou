#ifndef CAMERA_MOVEMENT_SYSTEM_H
#define CAMERA_MOVEMENT_SYSTEM_H

#include <memory>
#include <spdlog/spdlog.h>

#include "components/camera_follower_component.hpp"
#include "components/transform_component.hpp"
#include "ecs/system.hpp"
#include "game/camera.hpp"
#include "game/game.hpp"

class CameraMovementSystem : public System {

public:
  CameraMovementSystem() {
    requiredComponent<CameraFollowerComponent>();
    requiredComponent<TransformComponent>();
  }

  void update(Camera &camera) {

    for (auto &gameObject : gameObjects()) {

      const auto transform = gameObject.getComponent<TransformComponent>();


      if (transform.position.x < Game::MAP_DIMENSION.width) { // NOLINT
        camera.setX( static_cast<int32_t>(transform.position.x)); // NOLINT
      }


     if (transform.position.y < Game::MAP_DIMENSION.height) { //NOLINT
        camera.setY( static_cast<int32_t>(transform.position.y)); // NOLINT
      }
        /* keep camera within limits */
        camera.setX(camera.x()  < 0 ? 0 : camera.x());
        camera.setY(camera.y()  < 0 ? 0 : camera.y());
        camera.setX(camera.x() > camera.width() ? camera.width() : camera.x());
        camera.setY(camera.y() > camera.height() ? camera.height() : camera.y());
    }
  }

  [[nodiscard]] auto name() const -> std::string override;
};

#endif
