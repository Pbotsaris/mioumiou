#include "level.hpp"

Level *Level::m_instance = nullptr; // NOLINT
                                  

auto Level::getInstance() -> Level* {
   if (m_instance == nullptr) {
      m_instance = new Level(); // NOLINT
    }
    return m_instance;
}

void Level::cleanup(){
  delete m_instance; // NOLINT

}

void Level::setMap(Map &&map) { m_map = map; }

auto Level::getMap() -> Map { return m_map; }

void Level::setLevel(int32_t level) { m_level = level; }

auto Level::getLevel() const -> int32_t { return m_level; }
auto Level::isLoaded() const -> bool { return m_loaded; }
