#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include <cstdint>

#include"utils/configurables.hpp"

struct HealthComponent {
  std::int32_t currentHealth; // TODO: in units of health
  std::int32_t maxHealth;

  explicit HealthComponent(std::int32_t health = configurables::Player::INITIAL_MAX_HEALTH)
    : currentHealth(health), maxHealth(health){
  }
};


#endif
