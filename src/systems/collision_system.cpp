#include "collision_system.hpp"

auto CollisionSystem::name() const -> std::string { return "CollisionSystem"; }

auto CollisionSystem::hasCollidedLeftBottom(const Position &current, // NOLINT
                                            const Position &next) const
    -> bool { // NOLINT

  return next.origin.x >= current.origin.x && //NOLINT
         next.origin.x <= current.end.x && // NOLINT
         next.end.y >= current.origin.y && //NOLINT
         next.end.y <= current.end.y; // NOLINT
}

auto CollisionSystem::hasCollidedLeftTop(const Position &current, // NOLINT
                                         const Position &next) const
    -> bool { // NOLINT

  return next.origin.x >= current.origin.x && //NOLINT
         next.origin.x <= current.end.x && // NOLINT
         next.origin.y >= current.origin.y && //NOLINT
         next.origin.y <= current.end.y; // NOLINT
}

auto CollisionSystem::hasCollidedRightBottom(const Position &current, // NOLINT
                                             const Position &next) const
    -> bool { // NOLINT

  return next.end.x >= current.origin.x && //NOLINT
         next.end.x <= current.end.x && // NOLINT
         next.end.y >= current.origin.y && //NOLINT
         next.end.y <= current.end.y; // NOLINT
}

auto CollisionSystem::hasCollidedRightTop(const Position &current, // NOLINT
                                          const Position &next) const
    -> bool { // NOLINT

  return next.end.x >= current.origin.x && //NOLINT
         next.end.x <= current.end.x && // NOLINT
         next.origin.y >= current.origin.y && //NOLINT
         next.origin.y <= current.end.y; // NOLINT
}
