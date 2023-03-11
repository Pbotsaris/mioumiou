#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

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
};

#endif
