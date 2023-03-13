#ifndef RENDER_GUI_SYSTEM_H
#define RENDER_GUI_SYSTEM_H
#include <memory>

#include "ecs/system.hpp"
#include "ecs/world_manager.hpp"
#include "game/asset_store.hpp"
#include "game/camera.hpp"

class RenderGuiSystem : public System{

  public:
  /* Does not require components */
  RenderGuiSystem() = default;

  void update(std::unique_ptr<WorldManager> &worldManager, std::unique_ptr<AssetStore> &store, Camera &camera);

  [[nodiscard]] auto name() const -> std::string override;
};

#endif
