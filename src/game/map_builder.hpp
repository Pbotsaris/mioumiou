#ifndef MAP_BUILDER_H
#define MAP_BUILDER_H
#include "ecs/world_manager.hpp"

#include <cstdint>
#include <string>

constexpr const static uint32_t DEFAULT_TILE_SIZE = 32;

struct TileDimension {
   int32_t width = DEFAULT_TILE_SIZE;
   int32_t height = DEFAULT_TILE_SIZE;
   int32_t scale = 1;
};

class MapBuilder {

public:
  explicit MapBuilder(std::string path, std::string storeKey, const TileDimension &tile, std::string delim = ",");
  void build(std::unique_ptr<WorldManager> &wm); //NOLINT

private:
  std::string         m_mapPath;
  std::string         m_storeKey;
  const TileDimension &m_tile;
  std::string         m_delim;

void loadTile(std::unique_ptr<WorldManager> &wm, const std::string &value, glm::vec2 position) const; //NOLINT

};

#endif
