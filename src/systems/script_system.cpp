#include "script_system.hpp"
#include "ecs/world_manager.hpp"
#include <SDL2/SDL.h>

auto ScriptSystem::name() const -> std::string { return "ScriptSystem"; }

void ScriptSystem::update(double delta){
  for(auto gameObject : gameObjects()){
    const auto script = gameObject.getComponent<ScriptComponent>();
    script.onUpdate(gameObject, delta, SDL_GetTicks());
  }
}

void ScriptSystem::createLuaBindings(sol::state &lua){

   lua.new_usertype<GameObject>(
       "game_object",
       "id",          &GameObject::id,
       "remove",      &GameObject::remove,
       "tag",         &GameObject::tag,
       "untag",       &GameObject::removeTag,
       "group",       &GameObject::toGroup,
       "ungroup",     &GameObject::ungroup,
       "belongs_to",  &GameObject::belongsTo,
       "transform",   &GameObject::getComponent<TransformComponent>
       );

  lua.set_function("set_position", setPosition);

};


/* Lua binds */

void ScriptSystem::setPosition(GameObject gameObject, double x, double y){ // NOLINT
  
  if(gameObject.hasComponent<TransformComponent>()){
    
    auto &transform = gameObject.getComponent<TransformComponent>();
    transform.position = {x, y};
  }
}



