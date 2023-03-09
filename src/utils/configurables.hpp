#ifndef CONFIGURABLES_H
#define CONFIGURABLES_H

#include <string>


/* 
 *  This files contains values that will be configurable by the users
 *  Approach TBD
 *
 * */

namespace configurables {
  struct Tags{
    constexpr static std::string PLAYER = "player";
  };

  struct Groups {
    constexpr static std::string ENEMIES = "enemies";
    constexpr static std::string PROJECTILES = "projectiles";
  };

  struct Alliances {
    constexpr static std::string ENEMIES = "enemies";
    constexpr static std::string NEUTRAL = "neutral";
    constexpr static std::string PLAYER = "player";
  };

  struct Player {
    constexpr static std::uint32_t INITIAL_MAX_HEALTH = 100;
  };
};

#endif
