#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "components/boxcollider_component.hpp"
#include "components/transform_component.hpp"
#include "ecs/system.hpp"
#include <functional>
#include <spdlog/spdlog.h>

class CollisionSystem : public System {

  struct Position {
    glm::vec2 origin;
    glm::vec2 end;
    Position(glm::vec2 origin, glm::vec2 size) : origin(origin), end(size){}; // NOLINT
  };

public:
  CollisionSystem() {
    requiredComponent<BoxColliderComponent>();
    requiredComponent<TransformComponent>();
  }

  void update() {
    auto objs = gameObjects();

    for (auto current = objs.begin(); current < objs.end(); current++) {

      auto transform = current->getComponent<TransformComponent>();
      auto collider = current->getComponent<BoxColliderComponent>();

     Position currentPos(transform.position + collider.offset,
                          transform.position + collider.size + collider.offset);

      verifyCollision(objs.end(), currentPos, current);
    };
  };

  [[nodiscard]] auto name() const -> std::string override;

private:
  void verifyCollision(const std::vector<GameObject>::iterator end,
                        Position &currentPos,
                       std::vector<GameObject>::iterator current) const {

    for (auto next = current + 1; next < end; next++) {

      auto transform = next->getComponent<TransformComponent>();
      auto collider = next->getComponent<BoxColliderComponent>();

      Position nextPos(transform.position + collider.offset,
                       transform.position + collider.size + collider.offset);

      if (hasCollidedLeftBottom(currentPos, nextPos)) {
        spdlog::warn("has collided left bottom.");
      }

      if (hasCollidedLeftTop(currentPos, nextPos)) {
        spdlog::warn("has collided left top.");
      }

      if (hasCollidedRightBottom(currentPos, nextPos)) {
        spdlog::warn("has collided right bottom.");
      }

      if (hasCollidedRightTop(currentPos, nextPos)) {
        spdlog::warn("has collided right top.");
      }
    }
  }

  [[nodiscard]] auto hasCollidedLeftBottom(const Position &current,
                                           const Position &next) const -> bool;

  [[nodiscard]] auto hasCollidedLeftTop(const Position &current,
                                        const Position &next) const -> bool;

  [[nodiscard]] auto hasCollidedRightBottom(const Position &current,
                                            const Position &next) const -> bool;

  [[nodiscard]] auto hasCollidedRightTop(const Position &current,
                                         const Position &next) const -> bool;
};

#endif
