#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "types/containers.hpp"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <array>
#include <glm.hpp>
#include <iostream>

namespace constants {

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

struct ProjectPaths {
  constexpr static const char *ASSETS = "assets";
  constexpr static const char *SCRIPT = "script";
  constexpr static const char *SCRIPT_LEVEL = "script/level";
  constexpr static const char *SCRIPT_BEHAVIOUR = "script/behaviour";
};

struct Time {
  constexpr static const double MILLIS_IN_SEC = 1000.0;
};

struct Files {
  constexpr static const char *CONFIG_FILE = "config.lua";
};

struct Position {
  constexpr static const double Z_MAX = 50;
  constexpr static const double Z_MIN = 0;
  constexpr static const double Z_NOTMAP = 1;
};

struct Percentage {
  constexpr static const std::int32_t DIVISOR = 100;
};

struct Pool {
  constexpr static const std::int32_t COMPONENT_POOL_SIZE = 100;
};

struct Tags {
  constexpr static const char *MAIN_PLAYER = "player";
};

struct Lua {

  struct GenericTable {
    struct Size {
      constexpr static const char *WIDTH = "width";
      constexpr static const char *HEIGHT = "height";
    };

    struct XY {
      constexpr static const char *X = "x"; // NOLINT
      constexpr static const char *Y = "y"; // NOLINT
    };
  };


  struct Callbacks{
    constexpr static const char *ON_CREATE = "onCreate";
    constexpr static const char *ON_UPDATE = "onUpdate";
    constexpr static const char *ON_DESTROY = "onDestroy";

  };

  constexpr static const char *PROJECT = "Project";

  struct Project {
     constexpr static const char *NAME   = "name";
     constexpr static const char *PATH   = "path";
     constexpr static const char *WINDOW = "window";
     constexpr static const char *FPS    = "fps";
  };

  constexpr static const char *LEVEL = "Level";

  struct Level {

    /* Map */
    constexpr static const char *MAP = "map";

    struct Map {
      constexpr static const char *PATH = "path";
      constexpr static const char *TEXTURE_NAME = "texture_name";
      constexpr static const char *NB_ROWS = "num_rows";
      constexpr static const char *NB_COLS = "num_cols";
      constexpr static const char *TILE_WIDTH = "tile_width";
      constexpr static const char *TILE_HEIGHT = "tile_height";
      constexpr static const char *SCALE = "scale";
      constexpr static const char *PADDING = "padding";

      struct Padding {
        constexpr static const char *TOP = "top";
        constexpr static const char *RIGHT = "right";
        constexpr static const char *BOTTOM = "bottom";
        constexpr static const char *LEFT = "left";
      };
    };

    /* Game Objects */
    constexpr static const char *GAME_OBJECTS = "game_objects";

    struct GameObject {
      constexpr static const char *NAME = "name";
      constexpr static const char *TAG = "tag";
      constexpr static const char *GROUPS = "groups";
      constexpr static const char *ALLIANCES = "alliances";
      constexpr static const char *COMPONENTS = "components";

      struct Components {
        constexpr static const char *TRANSFORM = "transform";

        struct Transform {
          constexpr static const char *POSITION = "position";
          constexpr static const char *SCALE = "scale"; // NOLINT
          constexpr static const char *ROTATION = "rotation";
        };

        constexpr static const char *RIGID_BODY = "rigid_body";

        struct RigidBody {
          constexpr static const char *VELOCITY = "velocity";
        };

        constexpr static const char *SPRITE = "sprite";

        struct Sprite {
          constexpr static const char *ASSET_NAME = "asset_name";

          constexpr static const char *SIZE = "size";
          constexpr static const char *Z_POS = "z_position";
          constexpr static const char *SHEET = "sheet";
          constexpr static const char *FLIPABLE = "flippable";
          constexpr static const char *FIXED = "fixed";
          constexpr static const char *CROP = "crop";

          struct Crop {
            constexpr static const char *X = "x"; // NOLINT
            constexpr static const char *Y = "y"; // NOLINT
            constexpr static const char *WIDTH = "width";
            constexpr static const char *HEIGHT = "height";
          };
        };

        constexpr static const char *ANIMATION = "animation";

        struct Animation {
          constexpr static const char *FRAMES = "frames";
          constexpr static const char *RATE = "rate";
          constexpr static const char *LOOPABLE = "loopable";
        };

        constexpr static const char *BOX_COLLIDER = "boxcollider";

        struct BoxCollider {
          constexpr static const char *SIZE = "size";
          constexpr static const char *OFFSET = "offset";
        };

        constexpr static const char *HEALTH = "health";

        struct Health {
          constexpr static const char *CURRENT = "current";
          constexpr static const char *MAX = "max";
        };

        constexpr static const char *HEALTH_BAR = "health_bar";

        struct HealthBar {
          constexpr static const char *VISIBLE = "visible";
          constexpr static const char *SIZE = "size";
          constexpr static const char *OFFSET = "offset";
          constexpr static const char *ALIGMENT = "aligment";

          constexpr static const std::array<const char *, 4> ALIGN_OPS = {
              "top",
              "right",
              "bottom",
              "left",
          };
          constexpr static const char *ALIGN_OPS_MSG =
              "'top', 'right', 'bottom' or 'left'";

          constexpr static const char *TEXT = "text";

          struct Text {
            constexpr static const char *VISIBLE = "visible";
            constexpr static const char *FONT = "font";
            constexpr static const char *MARGIN = "margin";
            constexpr static const char *FONT_SIZE = "font_size";
            constexpr static const std::array<const char *, Fonts::NB_SIZES>
                FONT_SIZE_OPS = {
                    "tiny",  "extra-small", "medium",
                    "large", "extra-large", "huge",
            }; // FONT_SIZE_OPTS

            constexpr static const char *FONT_SIZE_MSG =
                "'tiny', 'extra-small', 'medium', 'large', 'extra-large' or "
                "'huge'";
          }; // Text

        }; // HealthBar

        constexpr static const char *PROJECTILE_EMITTER = "projectile_emitter";

        struct ProjectileEmitter {
          constexpr static const char *SPRITE = "sprite";
          constexpr static const char *SIZE = "size";
          constexpr static const char *VELOCITY = "velocity";
          constexpr static const char *FREQUENCY = "frequency";
          constexpr static const char *LONGEVITY = "longevity";
          constexpr static const char *PERCENT_DAMAGE = "percent_damage";
          constexpr static const char *ORIGIN_OFFSET = "origin_offset";
          constexpr static const char *CONTROL = "control";

          struct Control {
            constexpr static const char *CONTROLLABLE = "controllable";
            constexpr static const char *KEY = "key";
          };
        };

        constexpr static const char *KEYBOARD_CONTROL = "keyboard_control";

        struct KeyboardControl {
          constexpr static const char *UP_VELOCITY = "up_velocity";
          constexpr static const char *RIGHT_VELOCITY = "right_velocity";
          constexpr static const char *DOWN_VELOCITY = "down_velocity";
          constexpr static const char *LEFT_VELOCITY = "left_velocity";
          constexpr static const char *STRATEGY = "strategy";

          constexpr static const std::array<const char *, 2> STRATEGY_OPS = {
              "spritesheet", "rotation"};

          constexpr static const char *STRATEGY_MESSAGE =
              "'rotation' and 'spritesheet'";
        };
        constexpr static const char *CAMERA_FOLLOW = "camera_follow";

        struct CameraFollow {
          constexpr static const char *FOLLOW = "follow";
        };

        constexpr static const char *SCRIPT = "script";

        struct Script {
          constexpr static const char *PATH = "path";
        };
      };
    };

    /* Assets */
    constexpr static const char *ASSETS = "assets";

    struct Assets {
      constexpr static const char *TYPE = "type";
      constexpr static const char *NAME = "name";
      constexpr static const char *FILE = "file";

      struct Type {
        constexpr static const char *TEXTURE = "texture";
        constexpr static const char *FONT = "font";
      };
    };
  };
};

struct Defaults {

  struct Resolution {
    constexpr static size_t WINDOW_WIDTH = 1280;
    constexpr static size_t WINDOW_HEIGHT = 1024;
    constexpr static int32_t FPS = 60;
  };

  struct Project {
    constexpr static const char* NAME = "untitled";
  };

  struct Health {
    constexpr static const std::int32_t CURRENT_HEALTH = 100;
    constexpr static const std::int32_t MAX_HEALTH = 100;
  };

  struct Info {
    constexpr static const char *NAME = "unnamed";
  };

  struct HealthBar {
    constexpr static const std::int32_t DEFAULT_WIDTH = 20;
    constexpr static const std::int32_t DEFAULT_HEIGHT = 5;
    constexpr static const bool VISIBLE = true;
    constexpr static const glm::vec2 SIZE = {DEFAULT_WIDTH, DEFAULT_HEIGHT};
    constexpr static const glm::vec2 OFFSET = {0, 0};
    constexpr static const int32_t ALIGNMENT = 3; // BOTTOM
                                                  //
    constexpr static const bool TEXT_VISIBLE = true;
    constexpr static const Fonts::Size TEXT_FONT_SIZE = Fonts::Tiny;
    constexpr static const char *TEXT_FONT = "charriot";
    constexpr static const int32_t TEXT_MARGIN = 0;
  };

  struct Transform {
    constexpr static const glm::vec2 POSITION = {0, 0};
    constexpr static const glm::vec2 SCALE = {1, 1};
    constexpr static const double ROTATION = 0.0F;
  };

  struct Sprite {
    constexpr static const glm::vec2 SIZE = {0, 0};
    constexpr static const int32_t Z_POSITION = Position::Z_NOTMAP;
    constexpr static const bool IS_SHEET = false;
    constexpr static const bool FLIPABLE = true;
    constexpr static const bool IS_FIXED = false;
    constexpr static const SDL_Rect CROP = {0, 0, 0, 0};
    constexpr static const SDL_RendererFlip FLIP = SDL_FLIP_NONE;
    constexpr static const int32_t ORIENTATION = 0;
  };

  struct RigidBody {
    constexpr static const glm::vec2 VELOCITY = {0, 0};
  };

  struct Animation {
    constexpr static const int32_t NB_FRAMES = 1;
    constexpr static const int32_t FRAME_RATE_SPEED = 1;
    constexpr static const bool LOOPABLE = true;
  };

  struct BoxCollider {
    constexpr static const glm::vec2 SIZE = {0, 0};
    constexpr static const glm::vec2 OFFSET = {0, 0};
  };

  struct ProjectileEmitter {
    constexpr static const glm::vec2 SIZE = {0, 0};
    constexpr static const glm::vec2 VELOCITY = {0.0, 0.0};
    constexpr static const uint32_t FREQUENCY = 500;
    constexpr static const uint32_t LONGEVITY = 500;
    constexpr static const int32_t PERCENT_DAMAGE = 10;
    constexpr static const glm::vec2 OFFSET = {0, 0};
    constexpr static const SDL_KeyCode CONTROL_KEY = SDLK_SPACE;
  };

  struct KeyboardControl {
    constexpr static const double UP_VELOCITY = 0.0F;
    constexpr static const double RIGHT_VELOCITY = 0.0F;
    constexpr static const double DOWN_VELOCITY = 0.0F;
    constexpr static const double LEFT_VELOCITY = 0.0F;
    constexpr static const int32_t STRATEGY = 0;
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
