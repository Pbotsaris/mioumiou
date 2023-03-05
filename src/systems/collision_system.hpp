#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "components/all.hpp"
#include "ecs/system.hpp"
#include "event_system/event_bus.hpp"
#include "events/collision_event.hpp"
#include <functional>
#include <spdlog/spdlog.h>

class CollisionSystem : public System { //NOLINT

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


  void update(std::unique_ptr<EventBus> &eventBus) {
    auto objs = gameObjects();

    for (auto current = objs.begin(); current < objs.end(); current++) {

      auto transform = current->getComponent<TransformComponent>();
      auto collider = current->getComponent<BoxColliderComponent>();

      Position currentPos(transform.position + collider.offset,
                          transform.position + collider.size + collider.offset);

      verifyCollision(eventBus, objs.end(), currentPos, current);
    };
  };

  [[nodiscard]] auto name() const -> std::string override;

private:
  void verifyCollision(
                      std::unique_ptr<EventBus> &eventBus,
                      const std::vector<GameObject>::iterator end,
                      Position &currentPos,
                       std::vector<GameObject>::iterator current) const {

    for (auto next = current + 1; next < end; next++) {

      const auto transform = next->getComponent<TransformComponent>();
      const auto collider = next->getComponent<BoxColliderComponent>();

      Position nextPos(transform.position + collider.offset,
                       transform.position + collider.size + collider.offset);

      if (hasCollidedLeftBottom(currentPos, nextPos) ||
          hasCollidedLeftTop(currentPos, nextPos) ||
          hasCollidedRightBottom(currentPos, nextPos) ||
          hasCollidedRightTop(currentPos, nextPos)) {

        eventBus->dispatchEvent<CollisionEvent>(*current, *next);
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
