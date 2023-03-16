#include "level.hpp"

Level *Level::instance = nullptr; // NOLINT

void Level::setMap(Map &&map) { m_map = map; }

auto Level::getMap() -> Map { return m_map; }

void Level::setLevel(int32_t level) { m_level = level; }

auto Level::getLevel() const -> int32_t { return m_level; }
auto Level::isLoaded() const -> bool { return m_loaded; }
