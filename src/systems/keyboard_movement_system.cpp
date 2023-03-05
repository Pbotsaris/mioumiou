#include "keyboard_movement_system.hpp"
#include <spdlog/spdlog.h>

auto KeyboardMovementSystem::name() const -> std::string { return "KeyboardMovementSystem"; };


void KeyboardMovementSystem::onKeyPressed(KeyPressEvent &event){ //NOLINT

  spdlog::debug("keypress dispatched:  key [{}]->'{}' with modifier {}", event.key(), event.keyChar(), event.modifier());
}


