#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H
#include "ecs/world_manager.hpp"


#include <cstdint>
#include <string>

class MapBuilder {

  constexpr const static uint32_t DEFAULT_TILE_SIZE = 32;
  constexpr const static uint32_t DEFAULT_TILE_SCALE = 1;

  struct TitleDimensions{
    int32_t size = DEFAULT_TILE_SIZE;
    int32_t scale = DEFAULT_TILE_SCALE;
  };

public:

  explicit MapBuilder(std::string path, std::string storeKey, std::string delim = ",");
  explicit MapBuilder(std::string path, std::string storeKey, TitleDimensions tile, std::string delim = ",");
  void build(std::unique_ptr<WorldManager> &wm); //NOLINT



private:
  std::string m_mapPath;
  std::string m_storeKey;
  TitleDimensions m_tile;

  std::string m_delim;


void loadTile(std::unique_ptr<WorldManager> &wm, const std::string &value, glm::vec2 position) const; //NOLINT

};

#endif
