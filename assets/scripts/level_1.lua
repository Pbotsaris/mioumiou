local assets = {
  tile = "map-tiles",
  chopper = "chooper",
  tank = "tank",
  bullet = "bullet",
  font = "arial",
}

local font_sizes = {
  tiny = "tiny",
  extraSmall = "extraSmall",
  medium = "medium",
  large = "large",
  extraLarge = "extraLarge",
  huge = "huge",
}

Level = {
  ----------------------------------------------------
  -- Table to define the list of assets
  ----------------------------------------------------
  assets = {
    [0] =
    { type = "texture", name = assets.tile, file = "./assets/tilemaps/jungle.png" },
    { type = "texture", name = assets.chopper, file = "./assets/images/chopper-green-spritesheet.png" },
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
    padding = {top = 10.0, right = 40.0, bottom = 40.0, left = 10},
  },

  ----------------------------------------------------
  -- table to define GameObjects and their components
  ----------------------------------------------------
  --
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
        rigidbody = {
          velocity = { x = 0.0, y = 0.0 }
        },
        sprite = {
          key = assets.chopper,
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
          width = 32,
          height = 25,
          offset = { x = 0, y = 5 }
        },
        health = {
          current = 100,
          max = 100,
        },
        health_bar = {
          visible = true,
          size = { width = 20, height = 5 },
          text = {
            visible = true,
            font = assets.font,
            font_size = font_sizes.tiny,
          },
          aligment = "bottom",
        },

        projectile_emitter = {
          sprite = assets.bullet,
          size = { width = 4, height = 4 },
          velocity = { x = 200, y = 200 },
          frequency = 0, -- seconds
          longevity = 10, -- seconds
          percent_damage = 10,
          controlable = true,
        },

        keyboard_controller = {
          up_velocity = { x = 0, y = -50 },
          right_velocity = { x = 50, y = 0 },
          down_velocity = { x = 0, y = 50 },
          left_velocity = { x = -50, y = 0 }
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
          texture_asset_id = assets.tank,
          size = { width = 32, height = 32 },
          z_position = 2,
          sheet = false,
          flippable = true,
          fixed = false,
          crop = { x = 0, y = 0, width = 32, height = 32 }
        },
        boxcollider = {
          width = 25,
          height = 18,
          offset = { x = 0, y = 7 }
        },
        health = {
          current = 100,
          max = 100,
        },
        projectile_emitter = {

          sprite = assets.bullet,
          size = { width = 4, height = 4 },
          velocity = { x = 100, y = 0 },
          frequency = 1, -- seconds
          longevity = 2, -- seconds
          percent_damage = 20,
          controlable = false,
        }
      }
    }
  }
}
