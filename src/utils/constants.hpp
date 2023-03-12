#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>
#include <glm.hpp>
#include <array>

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

  struct Fonts {
    constexpr static const size_t NB_SIZES = 7;

    enum Size {
      Tiny       = 0,
      ExtraSmall = 1,
      Small      = 2,
      Medium     = 3,
      Large      = 4,
      ExtraLarge = 5,
      Huge       = 6
    };

    constexpr static const std::array<std::int32_t, NB_SIZES> SIZES = {
      8,   /* Tiny */
      12,  /* ExtraSmall */
      16,  /* Small */
      20,  /* Medium */
      24,  /* Large */
      28,  /* ExtraLarge */
      36,  /* Huge */
    };

  };

  struct Defaults {
   struct HealthBar {
    constexpr static const std::int32_t DEFAULT_WIDTH     = 20;
    constexpr static const std::int32_t DEFAULT_HEIGHT    = 5;
    constexpr static const glm::vec2    SIZE              = {DEFAULT_WIDTH, DEFAULT_HEIGHT};
    constexpr static const Fonts::Size  DEFAULT_FONT_SIZE = Fonts::Tiny;
    constexpr static const std::string  DEFAULT_FONT      = "charriot";
   };
  };

};

#endif
