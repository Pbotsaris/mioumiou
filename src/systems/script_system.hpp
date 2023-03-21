#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "ecs/system.hpp"
#include "components/script_component.hpp"

class ScriptSystem : public System {

  public:
  ScriptSystem(){
    requiredComponent<ScriptComponent>();
  };

  static void createLuaBindings(sol::state &lua);

  void update(double delta);

  [[nodiscard]] auto name() const -> std::string override;

  private:

  static void setPosition(GameObject gameObject, double x, double y); // NOLINT
};

#endif
