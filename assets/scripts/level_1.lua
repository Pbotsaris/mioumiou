local assets = {
  tile = "map-tiles",
  chopper = "chooper",
  tank = "tank",
  bullet = "bullet",
  font = "arial",
}

local font_sizes = {
  tiny = "tiny",
  extraSmall = "extra-small",
  medium = "medium",
  large = "large",
  extraLarge = "extra-large",
  huge = "huge",
}

local keyboard_control_stategy = {
  spritesheet = "spritesheet",
  rotation = "rotation",
};

Level = {
  ----------------------------------------------------
  -- Table to define the list of assets
  ----------------------------------------------------
  assets = {
    [0] =
    { type = "texture", name = assets.tile, file = "./assets/tilemaps/jungle.png" },
    { type = "texture", name = assets.chopper, file = "./assets/images/chopper-spritesheet.png" },
    { type = "texture", name = assets.tank, file = "./assets/images/tank-tiger-up.png" },
    { type = "texture", name = assets.bullet, file = "./assets/images/bullet.png" },
    { type = "font", name = assets.font, file = "./assets/fonts/pico8.ttf" },
  },

  ----------------------------------------------------
  -- table to define the map config variables
  ----------------------------------------------------
  map = {
    path = "./assets/tilemaps/jungle.map",
    texture_name = assets.tile,
    num_rows = 20,
    num_cols = 25,
    tile_width = 32,
    tile_height = 32,
    scale = 2.0,
    padding = { top = 10, right = 40, bottom = 40, left = 10 },
  },

  ----------------------------------------------------
  -- table to define GameObjects and their components
  ----------------------------------------------------

  game_objects = {
    [0] =
    {
      -- Player
      tag = "player",
      alliances = { "player", "neutral" },
      components = {
        transform = {
          position = { x = 242, y = 110 },
          scale = { x = 1.0, y = 1.0 },
          rotation = 0.0, -- degrees
        },
        rigid_body = {
          velocity = { x = 0.0, y = 0.0 }
        },
        sprite = {
          asset_name = assets.chopper,
          size = { width = 32, height = 32 },
          z_position = 4,
          sheet = true,
          flippable = false,
          fixed = false,
          crop = { x = 0, y = 0, width = 32, height = 32 }
        },
        animation = {
          frames = 2,
          rate = 10, -- fps
          loopable = true,
        },
        boxcollider = {
          size = { width = 32, height = 25 },
          offset = { x = 0, y = 5 }
        },
        health = {
          current = 100,
          max = 100,
        },
        health_bar = {
          visible = true,
          size = { width = 20, height = 5 },
          offset = { x = 0, y = 0 },
          aligment = "bottom",
          text = {
            visible = true,
            font = assets.font,
            font_size = font_sizes.tiny,
            margin = 6, --between healthbar and text, in pixels
          },
        },
        projectile_emitter = {
          sprite = assets.bullet,
          size = { width = 4, height = 4 },
          velocity = { x = 200, y = 200 },
          frequency = 0, -- millis
          longevity = 1000, -- millis
          percent_damage = 10,
          origin_offset = { x = 0, y = 0 },
          control = {
            controllable = true,
            key = ' ',
          }
        },

        keyboard_control = {
          up_velocity    = 100.0,
          right_velocity = 100.0,
          down_velocity  = 100.0,
          left_velocity  = 100.0,
          -- spritesheet will update direction using a spritesheet
          -- the rotation strategy will just rotate the sprite using SDL
          strategy       = keyboard_control_stategy.spritesheet,
        },

        camera_follow = {
          follow = true
        },
      }
    },
    {
      -- Tank
      alliances = { "enemies" },
      groups = { "bounderObject" },
      components = {
        transform = {
          position = { x = 200, y = 497 },
          scale = { x = 1.0, y = 1.0 },
          rotation = 0.0, -- degrees
        },
        sprite = {
          asset_name = assets.tank,
          size = { width = 32, height = 32 },
          z_position = 2,
          sheet = false,
          flippable = true,
          fixed = false,
          crop = { x = 0, y = 0, width = 32, height = 32 }
        },
        boxcollider = {
          size = { width = 32, height = 25 },
          offset = { x = 0, y = 7 }
        },
        health = {
          current = 100,
          max = 100,
        },
        health_bar = {
          visible = true,
          size = { width = 20, height = 5 },
          offset = { x = 0, y = -20 },
          aligment = "top",
          text = {
            visible = true,
            font = assets.font,
            font_size = font_sizes.tiny,
            margin = 6, --between healthbar and text, in pixels
          },
        },
        projectile_emitter = {
          sprite = assets.bullet,
          size = { width = 4, height = 4 },
          velocity = { x = 100, y = 0 },
          frequency = 500, -- seconds
          longevity = 2000, -- seconds
          percent_damage = 20,
          origin_offset = { x = 0, y = 0 },
        }
      }
    }
  }
}
