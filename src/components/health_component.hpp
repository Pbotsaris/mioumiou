#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include <cstdint>

#include"utils/constants.hpp"

struct HealthComponent {
  std::int32_t currentHealth; // TODO: in units of health
  std::int32_t maxHealth;

  explicit HealthComponent(
      std::int32_t health = constants::Defaults::Health::CURRENT_HEALTH, //NOLINT   
      std::int32_t maxHealth = constants::Defaults::Health::MAX_HEALTH
      )
    : currentHealth(health), maxHealth(maxHealth){
  }
};


#endif
