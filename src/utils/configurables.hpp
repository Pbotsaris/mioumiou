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

struct Resolution {
  constexpr static size_t WINDOW_WIDTH = 800;
  constexpr static size_t WINDOW_HEIGHT = 640;
  constexpr static int32_t FPS = 60;
};

struct Map {
  struct TileCount {
    constexpr static int32_t X_AXIS = 25;
    constexpr static int32_t Y_AXIS = 20;
  };

  // tile grows with Resolution
  struct TileDimension {
    constexpr static int32_t WIDTH = Resolution::WINDOW_WIDTH / TileCount::X_AXIS;
    constexpr static int32_t HEIGHT = Resolution::WINDOW_HEIGHT / TileCount::Y_AXIS;;
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
