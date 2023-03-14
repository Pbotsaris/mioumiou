#include "systems/movement_system.hpp"
#include "utils/configurables.hpp"

auto MovementSystem::name() const -> std::string { return "MovementSystem"; }

void MovementSystem::onCollision(CollisionEvent &event) { // NOLINT

  if (event.a().belongsTo(configurables::Groups::BOUNDER_OBJECTS) &&
      event.b().belongsTo(configurables::Groups::OBSTACLES)) {

    onEnemyHitsObstacle(event.a(), event.b());
  }

  if (event.a().belongsTo(configurables::Groups::OBSTACLES) &&
      event.b().belongsTo(configurables::Groups::BOUNDER_OBJECTS)) {

    onEnemyHitsObstacle(event.b(), event.a());
  }
};

void MovementSystem::onEnemyHitsObstacle(GameObject bounderObject, GameObject obstacle) { // NOLINT

  Direction direction = None;

  /* no velocity, do nothing */
  if (bounderObject.hasComponent<RigidBodyComponent>()) {
    auto &rigidBody = bounderObject.getComponent<RigidBodyComponent>();

    if (rigidBody.velocity.x != 0) { // NOLINT
      direction = Horizontal;
      rigidBody.velocity.x *= -1; // NOLINT
    }

    if (rigidBody.velocity.y != 0) { // NOLINT
      direction = direction != None ? Both : Vertical;
      rigidBody.velocity.y *= -1; // NOLINT
    }

    if (bounderObject.hasComponent<SpriteComponent>()) {
      auto &sprite = bounderObject.getComponent<SpriteComponent>();

      /* must be flippable */
      if(!sprite.flippable){
        return;
      }

      if (sprite.isSheet) { // rotation for spritesheets
        handleSpriteSheetFlip(sprite, direction);
        return;
      }

      if (direction == Horizontal || direction == Both) {
        sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        return;
      }

      if (direction == Vertical) {
        sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
        return;
      }
    }
  }
  spdlog::info("obstacle {}", obstacle.id());
}

void MovementSystem::handleSpriteSheetFlip(SpriteComponent &sprite, Direction direction) {

  switch (direction) {
  case Vertical:
    doSheetVertical(sprite);
    break;

  case Horizontal:
    doSheetHorizontal(sprite);
    break;

  case None:
    break;

  default: // Both just flip horizontally
    doSheetHorizontal(sprite);
  }
}

void MovementSystem::doSheetVertical(SpriteComponent &sprite) {
  if (sprite.orientation == SpriteComponent::Up) {
    sprite.crop.y = sprite.getHeight() * constants::SpriteSheet::LOOK_DOWN;
    sprite.orientation = SpriteComponent::Down;
    return;
  }

  sprite.crop.y = sprite.getHeight() * constants::SpriteSheet::LOOK_UP;
  sprite.orientation = SpriteComponent::Up;
}

void MovementSystem::doSheetHorizontal(SpriteComponent &sprite) {
  if (sprite.orientation == SpriteComponent::Right) {
    sprite.crop.y = sprite.getHeight() * constants::SpriteSheet::LOOK_LEFT;
    sprite.orientation = SpriteComponent::Left;
    return;
  }

  sprite.crop.y = sprite.getHeight() * constants::SpriteSheet::LOOK_RIGHT;
  sprite.orientation = SpriteComponent::Right;
}

auto MovementSystem::isOutBoundsX(const TransformComponent &transform) -> bool {
  return transform.position.x < 0 ||  // NOLINT
         transform.position.x > configurables::Map::Dimensions::WIDTH; // NOLINT
};

auto MovementSystem::isOutBoundsY(const TransformComponent &transform) -> bool{
    return transform.position.y < 0 || // NOLINT
    transform.position.y > configurables::Map::Dimensions::HEIGHT; // NOLINT
}

void MovementSystem::limitBounds(TransformComponent &transform){
   float width = configurables::Map::Dimensions::WIDTH;
   float height = configurables::Map::Dimensions::HEIGHT;
   auto padding = configurables::Map::BoundsPadding::PADDING;

   transform.position.x = transform.position.x < 0 + padding.left  ? 0 + padding.left : transform.position.x; //NOLINT
   transform.position.y = transform.position.y < 0 + padding.top ? 0 + padding.top : transform.position.y; // NOLINT
   transform.position.x = transform.position.x > width - padding.right ? width - padding.right : transform.position.x; // NOLINT
   transform.position.y = transform.position.y > height - padding.bottom ? height - padding.bottom : transform.position.y; //NOLINT
};


