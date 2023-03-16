#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include "ecs/world_manager.hpp"

#include <cstdint>
#include <string>
#include "utils/constants.hpp"


struct TileDimension {
   int32_t width = constants::Defaults::Map::Tile::WIDTH;
   int32_t height = constants::Defaults::Map::Tile::HEIGHT;
   float scale = 1.0;
};

class MapBuilder {

public:
  explicit MapBuilder(std::string path, std::string storetileName,
      const TileDimension &tile,
      std::string delim = constants::Defaults::Map::MAP_DELIM);

  void build(std::unique_ptr<WorldManager> &wm); //NOLINT

private:
  std::string         m_mapPath;
  std::string         m_storeKey;
  const TileDimension &m_tile;
  std::string         m_delim;

void loadTile(std::unique_ptr<WorldManager> &wm, const std::string &value, glm::vec2 position) const; //NOLINT

};

#endif
