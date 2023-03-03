#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

namespace constants {
  struct Time {
    constexpr static const std::uint32_t MILLI_TOSEC_MULT = 1000;
  };

};


#endif
