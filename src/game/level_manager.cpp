#include "level_manager.hpp"
#include "components/all.hpp"
#include "spdlog/spdlog.h"
#include "utils/configurables.hpp"
#include "utils/constants.hpp"
#include "map_builder.hpp"
#include "level.hpp"
#include "types/containers.hpp"

using namespace constants;
using namespace configurables;

LevelManager::LevelManager(){

};

LevelManager::~LevelManager() {}

void LevelManager::setLevel(sol::state &lua, const std::string &path, int32_t levelNumber) {

  if (!PathUtils::pathExists(path)) {
    spdlog::error("'{}' is not a valid path to a lua file", path);
    return;
  }

  m_path = path;
  m_levelNumber = levelNumber;

  
  sol::load_result res = lua.load_file(m_path);

  if (!res.valid()) {
    sol::error err = res;
    spdlog::error("There's been an error loading lua file '{}'. Error: \n", m_path, err.what());
  }

  lua.script_file(m_path);
  sol::optional<sol::table> maybe = lua["Level"];

  if (maybe == sol::nullopt) {
    spdlog::error( "Could not load '{}' file. Missing '{}' table. ", m_path, Lua::LEVEL);
    return;
  }

  Level *level = Level::getInstance();

  level->setLevel(levelNumber);
  m_luaLevel = maybe;
}

/* */

void LevelManager::loadAssets(std::unique_ptr<AssetStore> &store, std::unique_ptr<Renderer> &renderer) {

  if (m_luaLevel == sol::nullopt) {
    spdlog::error("Cannot load level '{}'. luaLevel was not loaded porperly.", m_levelNumber);
    return;
  }

  sol::optional<sol::table> assets = m_luaLevel.value()[Lua::Level::ASSETS];

  if (assets == sol::nullopt) {
    spdlog::warn(" In file '{}': '{}' table must have a '{}' key.)", m_path, Lua::LEVEL, Lua::Level::ASSETS);
    return;
  }

  int32_t index = 0;

  while (true) {
    sol::optional<sol::table> asset = assets.value()[index];

    if (asset == sol::nullopt) {
      break;
    }

    sol::optional<std::string> name = asset.value()[Lua::Level::Assets::NAME];
    sol::optional<std::string> file = asset.value()[Lua::Level::Assets::FILE];
    sol::optional<std::string> type = asset.value()[Lua::Level::Assets::TYPE];

    if (name == sol::nullopt || file == sol::nullopt || type == sol::nullopt) {
      spdlog::warn("In file '{}': Asset table must specify a '{}', '{}' and '{}. Skipping assets.'",
                   m_path, Lua::Level::Assets::NAME, Lua::Level::Assets::FILE,
                   Lua::Level::Assets::TYPE);
      index++;
      continue;
    }

    if (!PathUtils::pathExists(file.value())) {
      spdlog::error( "In file '{}'. Path '{}' does not exist. Could not load asset.",
          m_path, file.value());

      index++;
      continue;
    }

    /* Happy Path */

    if (type == Lua::Level::Assets::Type::TEXTURE) {
      store->loadTexture(renderer, name.value(), file.value());
      spdlog::info("Texture '{}' loaded successfully loaded to the AssetStore", name.value());
    }

    if (type == Lua::Level::Assets::Type::FONT) {
      store->loadFont(name.value(), file.value());

      spdlog::info("Font '{}' loaded successfully loaded to the AssetStore", name.value());
    }

   index++;
  }
}

/* */

void LevelManager::loadMap(std::unique_ptr<WorldManager> &wm){ // NOLINT
   
  sol::optional<sol::table> map = m_luaLevel.value()[Lua::Level::MAP];

  if(map == sol::nullopt){
    spdlog::error("In file '{}': The table '{}' does not exist. Could not load map.", m_path, Lua::Level::MAP);
    return;
  }
   sol::optional<std::string>mapPath = map.value()[Lua::Level::Map::PATH];
   sol::optional<std::string>texOpt = map.value()[Lua::Level::Map::TEXTURE_NAME];
   sol::optional<int32_t>numRowsOpt = map.value()[Lua::Level::Map::NB_ROWS];
   sol::optional<int32_t>numColsOpt = map.value()[Lua::Level::Map::NB_COLS];
   sol::optional<int32_t>tileWidthOpt = map.value()[Lua::Level::Map::TILE_WIDTH];
   sol::optional<int32_t>tileHeightOpt = map.value()[Lua::Level::Map::TILE_HEIGHT];
   sol::optional<float>scaleOpt = map.value()[Lua::Level::Map::SCALE];
   sol::optional<sol::table> paddingOpt = map.value()[Lua::Level::Map::PADDING];


   if(mapPath == sol::nullopt){
        spdlog::error("In File '{}': Must provide a path to a map file in table '{}'. Failed to load map.",
            m_path, Lua::Level::MAP);
      return;
   }

   std::string textureName = texOpt == sol::nullopt ? Defaults::Map::TILE_TEXTURE_NAME : texOpt.value();
   warn(texOpt, Lua::Level::Map::TEXTURE_NAME, Defaults::Map::TILE_TEXTURE_NAME);

   int32_t numRows = numRowsOpt == sol::nullopt ? Defaults::Map::NB_ROWS : numRowsOpt.value();
   warn(numRowsOpt, Lua::Level::Map::NB_ROWS, Defaults::Map::NB_ROWS);

   int32_t numCols = numColsOpt == sol::nullopt ? Defaults::Map::NB_COLS : numColsOpt.value();
   warn(numColsOpt, Lua::Level::Map::NB_COLS, Defaults::Map::NB_COLS);

   int32_t tileWidth = tileWidthOpt == sol::nullopt ? Defaults::Map::Tile::WIDTH : tileWidthOpt.value();
   warn(tileWidthOpt, Lua::Level::Map::TILE_WIDTH, Defaults::Map::Tile::WIDTH);

   int32_t tileHeight = tileHeightOpt == sol::nullopt ? Defaults::Map::Tile::HEIGHT : tileHeightOpt.value();
   warn(tileHeightOpt, Lua::Level::Map::TILE_HEIGHT, Defaults::Map::Tile::HEIGHT);

   float scale = scaleOpt == sol::nullopt ? Defaults::Map::Tile::SCALE : scaleOpt.value();
   warn(scaleOpt, Lua::Level::Map::SCALE, Defaults::Map::Tile::SCALE);
   
   TileDimension dim;
   dim.width = tileWidth;
   dim.height = tileHeight;
   dim.scale = scale;

   MapBuilder builder(mapPath.value(), textureName, dim);
   builder.build(wm);

   Level *level = Level::getInstance();

   level->setMap({
       .width =  numRows * static_cast<int32_t>(static_cast<float>(tileWidth) * scale),
       .height = numCols * static_cast<int32_t>(static_cast<float>(tileHeight) * scale),
       .padding = doPadding(paddingOpt),
       });
}


auto LevelManager::doPadding(sol::optional<sol::table> &paddingOpt) const -> Padding {

  if(paddingOpt == sol::nullopt){
    spdlog::warn( "In file '{}': Using default padding top = '{}' , right = '{}', bottom = '{}', right = '{}'.",
        m_path,
        Defaults::Map::BoundsPadding::PADDING.top,
        Defaults::Map::BoundsPadding::PADDING.right,
        Defaults::Map::BoundsPadding::PADDING.bottom,
        Defaults::Map::BoundsPadding::PADDING.left
        );

    return constants::Defaults::Map::BoundsPadding::PADDING;
  }

  sol::optional<float> topOpt = paddingOpt.value()[Lua::Level::Map::Padding::TOP];
  sol::optional<float> rightOpt = paddingOpt.value()[Lua::Level::Map::Padding::RIGHT];
  sol::optional<float> bottomOpt = paddingOpt.value()[Lua::Level::Map::Padding::BOTTOM];
  sol::optional<float> leftOpt = paddingOpt.value()[Lua::Level::Map::Padding::LEFT];

  Padding padding;

  padding.top = topOpt == sol::nullopt ? Defaults::Map::BoundsPadding::PADDING.top : topOpt.value();
  warn(topOpt, "Padding::" + Lua::Level::Map::Padding::TOP, Defaults::Map::BoundsPadding::PADDING.top);

  padding.top = rightOpt == sol::nullopt ? Defaults::Map::BoundsPadding::PADDING.right : rightOpt.value();
  warn(rightOpt, "Padding::" + Lua::Level::Map::Padding::RIGHT, Defaults::Map::BoundsPadding::PADDING.right);

  padding.top = bottomOpt == sol::nullopt ? Defaults::Map::BoundsPadding::PADDING.bottom : bottomOpt.value();
  warn(bottomOpt, "Padding::" + Lua::Level::Map::Padding::BOTTOM, Defaults::Map::BoundsPadding::PADDING.bottom);

  padding.top = leftOpt == sol::nullopt ? Defaults::Map::BoundsPadding::PADDING.left : leftOpt.value();
  warn(leftOpt, "Padding::" + Lua::Level::Map::Padding::LEFT, Defaults::Map::BoundsPadding::PADDING.left);

  return padding;
}



