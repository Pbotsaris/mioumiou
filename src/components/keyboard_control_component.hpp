#ifndef KEYBOARD_CONTROLLER_COMPONENT_H
#define KEYBOARD_CONTROLLER_COMPONENT_H

#include <glm.hpp>
#include "utils/constants.hpp"

struct KeyboardControlComponent {

  enum Strategy { 
    Rotation,      //  Rotates the game object
    SpriteSheet,   //  Cycles through a sprite sheet
  };

  struct Velocity {
    glm::vec2 up;
    glm::vec2 right;
    glm::vec2 down;
    glm::vec2 left;

    explicit Velocity(
        glm::vec2 up ,  //NOLINT
        glm::vec2 right, 
        glm::vec2 down ,
        glm::vec2 left
        )
        : up(up),
      right(right),
      down(down),
      left(left) {}
  };


  Velocity velocity;
  /* this defines how an object will be controlled by keyboard control system */
  Strategy strategy; 

  explicit KeyboardControlComponent(
      double up    = constants::Defaults::KeyboardControl::UP_VELOCITY,  //NOLINT
      double right = constants::Defaults::KeyboardControl::RIGHT_VELOCITY,
      double down  = constants::Defaults::KeyboardControl::DOWN_VELOCITY, 
      double left  = constants::Defaults::KeyboardControl::LEFT_VELOCITY, 
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
