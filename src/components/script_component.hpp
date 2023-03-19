#ifndef SCRIPT_COMPONENT_H
#define SCRIPT_COMPONENT_H

#include "utils/constants.hpp"
#include <sol.hpp>

struct ScriptComponent{
  sol::function onCreate;
  sol::function onUpdate;
  sol::function onDestroy;

  explicit ScriptComponent(
      sol::function onCreate =  sol::lua_nil, // NOLINT
      sol::function onUpdate =  sol::lua_nil,
      sol::function onDestroy = sol::lua_nil
      ) :
    onCreate(std::move(onCreate)),
    onUpdate(std::move(onUpdate)),
    onDestroy(std::move(onDestroy)){};
};


#endif
