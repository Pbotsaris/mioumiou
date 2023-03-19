#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H


#include "asset_store.hpp"
#include "ecs/world_manager.hpp"
#include "map_builder.hpp"
#include "renderer.hpp"
#include "utils/path.hpp"
#include "types/containers.hpp"
#include "utils/constants.hpp"
#include <cstdint>
#include <memory>
#include <sol.hpp>

class LevelManager {

public:
   explicit LevelManager(const std::string path, int32_t levelNumber);

   ~LevelManager();

  void loadAssets(std::unique_ptr<AssetStore> &store, std::unique_ptr<Renderer> &renderer);
  void loadMap(std::unique_ptr<WorldManager> &wm); //NOLINT
  void loadGameObjects(std::unique_ptr<WorldManager> &wm, sol::state &lua); //NOLINT

private:
  /* this class will hold to lua state just just to important a level, then throw it away */
  sol::state m_lua;
  sol::optional<sol::table> m_luaLevel;
  int32_t m_levelNumber = 0;
  std::string m_path;


  /* Lua tables */
  void doGroups(sol::table &groupTable, GameObject gameObject) const;
  void doAlliances(sol::table &allianceTable, GameObject gameObject) const;
  void doComponents(sol::table &componentsTable, GameObject gameObject) const;

  /* script */
  void doScript(sol::state &lua, sol::table &scriptTable, GameObject gameObject) const;
  static void bindScriptFunc(const std::string &path, sol::state &lua, GameObject gameObject);

  /* Components */
  void doTransform(sol::table &transformTable, GameObject gameObject) const;
  void doRigdBody(sol::table &rigidBodyTable, GameObject gameObject) const;
  void doSprite(sol::table &spriteTable, GameObject gameObject) const;
  void doAnimation(sol::table &animationTable, GameObject gameObject) const;
  void doBoxCollider(sol::table &boxColliderTable, GameObject gameObject) const;
  void doHealth(sol::table &healthTable, GameObject gameObject) const;
  void doHealthBar(sol::table &healthBarTable, GameObject gameObject) const;
  void doHealthBarText(sol::table &healthBarTextTable, GameObject gameObject) const;
  void doProjectileEmitter(sol::table &ProjEmitterTable, GameObject gameObject) const;
  void doProjectileEmitterControl(sol::table &ProjEmitterControlTable, GameObject gameObject) const;
  void doKeyboardControl(sol::table &keyControlTable, GameObject gameObject) const;
  void doCameraFollow(sol::table &camFollowTable, GameObject gameObject) const;
  /* helpers */

  auto doPadding(sol::optional<sol::table> &paddingOpt) const -> Padding;
  auto doWidthHeight(sol::table &widthHeightTable, glm::vec2 defaultValue, const char *tableKey) const -> glm::vec2;
  auto doCrop(sol::table &cropTable, SDL_Rect defaultValue, const char *tableKey) const -> SDL_Rect;


  /* Templated helpers */

  template <typename T>
  auto doXY(sol::table &XYtable, glm::vec2 defaultValue, const char *tableKey) const -> glm::vec2{

  glm::vec2 vec  = defaultValue;

  sol::optional<T> xOpt = XYtable[constants::Lua::GenericTable::XY::X];
  sol::optional<T> yOpt = XYtable[constants::Lua::GenericTable::XY::Y];

  if(xOpt != sol::nullopt){
    vec.x =  static_cast<float>(xOpt.value()); //NOLINT
  } else {
    spdlog::warn("In File: '{}': Missing 'x' key in '{}' . Loading default.", m_path, tableKey);
  }

  if(yOpt != sol::nullopt){
    vec.y =  static_cast<float>(yOpt.value()); //NOLINT
  } else {
    spdlog::warn("In File: '{}': Missing 'y' key value in '{}' . Loading default.", m_path, tableKey);
  }

  return vec;
}

  template <typename T>
  void warn(sol::optional<T> &opt, const std::string &name, T defaultVal) const {

    if (opt == sol::nullopt) {
      spdlog::warn( "In file '{}': Value for '{}' not present. Using default '{}'", m_path, name, defaultVal);
    }
  }
};

#endif
