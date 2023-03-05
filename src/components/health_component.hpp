#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include <cstdint>

struct HealthComponent {
  std::uint8_t healthPercentage;

  HealthComponent(std::uint8_t healthPercentage = 100): healthPercentage(healthPercentage){} //NOLINT
};



#endif
