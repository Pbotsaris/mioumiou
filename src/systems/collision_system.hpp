#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "components/all.hpp"
#include "ecs/system.hpp"
#include "event_system/event_bus.hpp"
#include "events/collision_event.hpp"
#include <functional>
#include <spdlog/spdlog.h>

class CollisionSystem : public System { //NOLINT

  /* 2 X x, y vectors representing the origin and the end of the side of a box collider */
  struct ObjectSide {
    glm::vec2 origin;
    glm::vec2 end;
    ObjectSide(glm::vec2 origin, glm::vec2 size) : origin(origin), end(size){}; // NOLINT
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

      ObjectSide currentPos(transform.position + collider.offset,
                          transform.position + collider.size + collider.offset);

      verifyCollision(eventBus, objs.end(), currentPos, current);
    };
  };

  [[nodiscard]] auto name() const -> std::string override;

private:
  void verifyCollision(
                      std::unique_ptr<EventBus> &eventBus,
                      const std::vector<GameObject>::iterator end,
                      ObjectSide &currentPos,
                       std::vector<GameObject>::iterator current) const {

    for (auto next = current + 1; next < end; next++) {

      const auto transform = next->getComponent<TransformComponent>();
      const auto collider = next->getComponent<BoxColliderComponent>();

      ObjectSide nextPos(transform.position + collider.offset,
                       transform.position + collider.size + collider.offset);

      if (hasCollidedLeftBottom(currentPos, nextPos) ||
          hasCollidedLeftTop(currentPos, nextPos) ||
          hasCollidedRightBottom(currentPos, nextPos) ||
          hasCollidedRightTop(currentPos, nextPos)) {

        eventBus->dispatchEvent<CollisionEvent>(*current, *next);
      }
    }
  }

  [[nodiscard]] auto hasCollidedLeftBottom(const ObjectSide &current,
                                           const ObjectSide &next) const -> bool;

  [[nodiscard]] auto hasCollidedLeftTop(const ObjectSide &current,
                                        const ObjectSide &next) const -> bool;

  [[nodiscard]] auto hasCollidedRightBottom(const ObjectSide &current,
                                            const ObjectSide &next) const -> bool;

  [[nodiscard]] auto hasCollidedRightTop(const ObjectSide &current,
                                         const ObjectSide &next) const -> bool;
};

#endif
