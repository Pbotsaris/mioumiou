#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>
#include <glm.hpp>
#include <iostream>
#include "types/containers.hpp"

namespace constants {
struct Time {
  constexpr static const double MILLIS_IN_SEC = 1000.0;
};

struct Percentage {
  constexpr static const std::int32_t DIVISOR = 100;
};

struct Pool {
  constexpr static const std::int32_t COMPONENT_POOL_SIZE = 100;
};

struct Tags {
  constexpr static const std::string MAIN_PLAYER = "player";
};

struct Lua {
    constexpr static const std::string LEVEL = "level";

  struct Level {

    /* Map */
    constexpr static const std::string MAP = "map";

    struct Map {
    constexpr static const std::string PATH = "path";
    constexpr static const std::string TEXTURE_NAME = "texture_name";
    constexpr static const std::string NB_ROWS = "num_rows";
    constexpr static const std::string NB_COLS = "num_cols";
    constexpr static const std::string TILE_WIDTH = "tile_width";
    constexpr static const std::string TILE_HEIGHT = "tile_height";
    constexpr static const std::string SCALE = "scale";
    constexpr static const std::string PADDING = "padding";

    struct Padding {
    constexpr static const std::string TOP = "top";
    constexpr static const std::string RIGHT = "right";
    constexpr static const std::string BOTTOM = "bottom";
    constexpr static const std::string LEFT = "left";
    };
        
    };

    /* Assets */
    constexpr static const std::string ASSETS = "assets";

    struct Assets {
      constexpr static const std::string TYPE = "type";
      constexpr static const std::string NAME = "name";
      constexpr static const std::string FILE = "file";

      struct Type {
        constexpr static const std::string TEXTURE = "texture";
        constexpr static const std::string FONT = "font";
      };
    };
  };
};

struct SpriteSheet {
  constexpr static const std::int32_t LOOK_UP = 0;
  constexpr static const std::int32_t LOOK_RIGHT = 1;
  constexpr static const std::int32_t LOOK_DOWN = 2;
  constexpr static const std::int32_t LOOK_LEFT = 3;
};

struct Fonts {
  constexpr static const size_t NB_SIZES = 7;

  enum Size {
    Tiny = 0,
    ExtraSmall = 1,
    Small = 2,
    Medium = 3,
    Large = 4,
    ExtraLarge = 5,
    Huge = 6
  };

  constexpr static const std::array<std::int32_t, NB_SIZES> SIZES = {
      8,  /* Tiny */
      12, /* ExtraSmall */
      16, /* Small */
      20, /* Medium */
      24, /* Large */
      28, /* ExtraLarge */
      36, /* Huge */
  };
};

struct Defaults {
  struct HealthBar {
    constexpr static const std::int32_t DEFAULT_WIDTH = 20;
    constexpr static const std::int32_t DEFAULT_HEIGHT = 5;
    constexpr static const glm::vec2 SIZE = {DEFAULT_WIDTH, DEFAULT_HEIGHT};
    constexpr static const Fonts::Size DEFAULT_FONT_SIZE = Fonts::Tiny;
    constexpr static const std::string DEFAULT_FONT = "charriot";
  };


struct Map {
  constexpr static std::string MAP_DELIM = ",";
  constexpr static std::string TILE_TEXTURE_NAME = "tile";

  constexpr static int32_t NB_ROWS = 25;
  constexpr static int32_t NB_COLS = 20;

  struct Tile {
    constexpr static int32_t WIDTH = 32;
    constexpr static int32_t HEIGHT = 32;
    constexpr static float SCALE = 2;
  };

  constexpr static int32_t WIDTH = NB_ROWS * Tile::WIDTH * Tile::SCALE;
  constexpr static int32_t HEIGHT = NB_COLS * Tile::HEIGHT * Tile::SCALE;

  // map bounds padding preventing player from going out of bounds
  struct BoundsPadding {
    constexpr static Padding PADDING = {
        .top = 10, .right = 40, .bottom = 40, .left = 10};
  };
};

};

}; // namespace constants

#endif
