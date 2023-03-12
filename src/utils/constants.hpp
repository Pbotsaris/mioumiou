#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <glm.hpp>
#include "types/types.hpp"

namespace constants {
  struct Time {
    constexpr static const double MILLIS_IN_SEC = 1000.0;
  };

  struct Percentage{
    constexpr static const std::int32_t DIVISOR = 100;
  };

  struct Pool {
    constexpr static const std::int32_t COMPONENT_POOL_SIZE = 100;
  };

  struct Defaults {
   struct HealthBar {
    constexpr static const std::int32_t DEFAULT_WIDTH = 20;
    constexpr static const std::int32_t DEFAULT_HEIGHT = 5;
    constexpr static const glm::vec2    SIZE = {DEFAULT_WIDTH, DEFAULT_HEIGHT};
    constexpr static const std::int32_t DEFAULT_FONT_SIZE = 12;
    constexpr static const std::string  DEFAULT_FONT = "charriot";
   };
  };

  struct Fonts {
    struct Size {
      constexpr static const Pixels TINY = 8;
      constexpr static const Pixels SMALL = 12;
      constexpr static const Pixels MEDIUM = 18;
      constexpr static const Pixels LARGE = 24;
      constexpr static const Pixels HUGE = 36;
    };
  };
};

#endif
