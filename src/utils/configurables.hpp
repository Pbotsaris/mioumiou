#ifndef CONFIGURABLES_H
#define CONFIGURABLES_H

#include <string>

/*
 *  This files contains values that will be configurable by the users
 *  Approach TBD
 *
 * */

namespace configurables {
struct Tags {
  constexpr static std::string MAIN_PLAYER = "player";
};

struct Groups {
  constexpr static std::string ENEMIES = "enemies";
  constexpr static std::string PROJECTILES = "projectiles";
  constexpr static std::string OBSTACLES = "obstacles";
  constexpr static std::string BOUNDER_OBJECTS = "bounder-objects";
  constexpr static std::string FLIPPABLE = "flipable";
};

struct Alliances {
  constexpr static std::string ENEMIES = "enemies";
  constexpr static std::string NEUTRAL = "neutral";
  constexpr static std::string PLAYER = "player";
};

struct Player {
  constexpr static std::uint32_t INITIAL_MAX_HEALTH = 100;
};

struct Resolution {
  constexpr static size_t WINDOW_WIDTH = 1280;
  constexpr static size_t WINDOW_HEIGHT = 1024;
  constexpr static int32_t FPS = 60;
};

struct Map {
  struct TileCount {
    constexpr static int32_t X_AXIS = 25;
    constexpr static int32_t Y_AXIS = 20;
  };

  struct TileDimension {
    constexpr static int32_t WIDTH =  32;
    constexpr static int32_t HEIGHT = 32;
    constexpr static int32_t SCALE = 2;
  };

  // map grows with TileDimension
  struct Dimensions {
    constexpr static int32_t WIDTH = TileCount::X_AXIS * TileDimension::WIDTH * TileDimension::SCALE;
    constexpr static int32_t HEIGHT = TileCount::Y_AXIS * TileDimension::HEIGHT * TileDimension::SCALE;
  };
};
}; // namespace configurables

#endif
