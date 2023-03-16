#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include "asset_store.hpp"
#include "ecs/world_manager.hpp"
#include "map_builder.hpp"
#include "renderer.hpp"
#include "utils/path.hpp"
#include "types/containers.hpp"
#include <cstdint>
#include <memory>
#include <sol.hpp>

class LevelManager {

public:
  LevelManager();

  void loadAssets(std::unique_ptr<AssetStore> &store, std::unique_ptr<Renderer> &renderer);
  void loadMap(std::unique_ptr<WorldManager> &wm);

  ~LevelManager();
  void setLevel(sol::state &lua, const std::string &path, int32_t levelNumber);

private:
  sol::optional<sol::table> m_luaLevel = sol::nullopt;
  int32_t m_levelNumber = 0;
  std::string m_path;

  auto doPadding(sol::optional<sol::table> &paddingOpt) const -> Padding;

  template <typename T>
  void warn(sol::optional<T> &opt, const std::string &name, T defaultVal) const {

    if (opt == sol::nullopt) {
      spdlog::warn( "In file '{}': Value for '{}' not present. Using default '{}'",
          m_path, name, defaultVal);
    }
  }
};

#endif
