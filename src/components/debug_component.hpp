#ifndef DEBUG_COMPONENT_H
#define DEBUG_COMPONENT_H

struct CollisionDebugger {
  bool isVisible = false;
  bool hasCollided = false;
};

struct DebugComponent{
  CollisionDebugger collision;

  explicit DebugComponent(CollisionDebugger collision
      = {.isVisible = false, .hasCollided = false}) :
    collision(collision){}
};


#endif
