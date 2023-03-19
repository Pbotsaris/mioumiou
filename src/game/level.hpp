#ifndef LEVEL_H
#define LEVEL_H

#include <cstdint>
#include "utils/constants.hpp"
#include "types/containers.hpp"

/* NOTE: Not a fan of singletons but need access to map dimensions throughout systems 
 * This singleton is only written in once and in one place (levelManager) when level is
 * loaded so it should be safe to be shared accross the app.
 *
 * */

class Level {
public:

  struct Map {
    int32_t width;
    int32_t height;
    Padding padding;
  };

private:
  static Level *m_instance; // NOLINT
  Map m_map;
  int32_t m_level;
  bool m_loaded;

  Level() : m_map({
      .width = constants::Defaults::Map::WIDTH,
      .height = constants::Defaults::Map::HEIGHT, 
      .padding = constants::Defaults::Map::BoundsPadding::PADDING
      }),
    m_level(0),
    m_loaded(false) {}

public:
  static Level *getInstance();

  static void cleanup();

  void setMap(Map &&map);

  auto getMap() -> Map;

  void setLevel(int32_t level);

  [[nodiscard]] auto getLevel() const -> int32_t;
  [[nodiscard]] auto isLoaded() const -> bool;
};

#endif
