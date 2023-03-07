#ifndef KEYBOARD_CONTROLLER_COMPONENT_H
#define KEYBOARD_CONTROLLER_COMPONENT_H

#include <glm.hpp>

struct KeyboardControlComponent {

  enum Strategy { 
    Rotation,      // Rotates the game object
    SpriteSheet,   //  Cycles through a sprite sheet
  };

  struct Velocity {
    glm::vec2 up;
    glm::vec2 right;
    glm::vec2 down;
    glm::vec2 left;

    explicit Velocity(
        glm::vec2 up =    {0, 0}, //NOLINT  Adjancent params similar type
        glm::vec2 right = {0, 0}, 
        glm::vec2 down  =  {0, 0},
        glm::vec2 left  =  {0, 0}) //NOLINT
        : up(up),
      right(right),
      down(down),
      left(left) {}
  };

  Velocity velocity;

  /* this defines how an object will be controlled by keyboard control system */
  Strategy strategy; 

  explicit KeyboardControlComponent(
      double up = 0.0, //NOLINT
      double right = 0.0,
      double down = 0.0,
      double left = 0.0,
      Strategy strategy = SpriteSheet
      ) 
      : velocity({0.0, up * -1},
                 {right, 0.0},
                 {0.0, down},
                 {left * -1, 0.0}),
        strategy(strategy) 
  {};

  /* constructor will create same velocity accross all axis */
 // explicit KeyboardControlComponent(double velocity = 0.0) :
 //   velocity({0.0, velocity * -1}, {velocity, 0.0}, {0.0, velocity}, {velocity * 1, 0.0}){};

};


#endif
