#include <fstream>

#include "components/all.hpp"
#include "map_builder.hpp"
#include "systems/all.hpp"
#include "utils/numbers.hpp"

MapBuilder::MapBuilder(std::string mapPath, std::string imageStoreKey,
                       const TileDimension &tile, std::string delim)
    : m_mapPath(std::move(mapPath)), m_storeKey(std::move(imageStoreKey)),
      m_tile(tile), m_delim(std::move(delim)) {}

void MapBuilder::build(std::unique_ptr<WorldManager> &wm) { // NOLINT

  std::string line;
  std::ifstream mapFile(m_mapPath);

  if (!mapFile) {
    spdlog::error("Failed to build map.Could not find provided path: '{}'.",
                  m_mapPath);
    return;
  }

  int32_t rowCount = 0;

  while (std::getline(mapFile, line)) {

    size_t linePos = 0;
    int32_t colCount = 0;

    while ((linePos = line.find(m_delim)) != std::string::npos) {
      std::string val = line.substr(0, linePos);

      if (val.size() < 2) {
        spdlog::error("map value {} should have 2 digits. Failed to load map.",
                      val);
        return;
      }

      loadTile(wm, val, {colCount * m_tile.width, rowCount * m_tile.height});

      line.erase(0, linePos + m_delim.length());
      colCount++;
    }

    loadTile(wm, line, {colCount * m_tile.width, rowCount * m_tile.height});

    rowCount++;
  }

  mapFile.close();
}
void MapBuilder::loadTile(std::unique_ptr<WorldManager> &wm, // NOLINT
                          const std::string &value, glm::vec2 position) const {

  auto [cropY, okY] = Numbers::fromChar<int32_t>(value[0]);
  auto [cropX, okX] = Numbers::fromChar<int32_t>(value[1]);

  if (!okX || !okY) {
    spdlog::error(
        "Unable converted map values '{},{}' to integer. Failed to build map.",
        value[0], value[1]);
    return;
  }

  auto mapTile = wm->createGameObject();
  glm::vec2 scale(m_tile.scale, m_tile.scale);

  mapTile.addComponent<TransformComponent>(position * scale, scale, 0.0);

  mapTile.addComponent<SpriteComponent>(
      m_storeKey,
      glm::vec2(m_tile.width, m_tile.height),
      0, // zPosition
      false, // isFixed
      SpriteComponent::makeCrop(m_tile.width * cropX, m_tile.height * cropY, m_tile.width, m_tile.height));

  mapTile.toGroup("tiles");
}
