#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer.h>
#include <imgui.h>

#include <glm.hpp>

#include "components/all.hpp"
#include "systems/gui/render_gui_system.hpp"
#include "utils/configurables.hpp"

using namespace configurables;

struct CreateEnemy {
  int32_t posX = 0;
  int32_t posY = 0;
  int32_t velX = 0;
  int32_t velY = 0;
};

auto RenderGuiSystem::name() const -> std::string { return "RenderGuiSystem"; }

void RenderGuiSystem::update(std::unique_ptr<WorldManager> &worldManager, std::unique_ptr<AssetStore> &store, // NOLINT
                              Camera &camera ) { // NOLINT
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  struct Example {
    int pos;
  };

  if (ImGui::Begin("Spawn Enemies")) {
    ImGui::Text("Click to spawn enemies."); // NOLINT

    static int32_t posX = 0;
    static int32_t posY = 0;
    static float objVelocityAngle = 0;
    static float obVelocitySpeed = 0;
    static float rotation = 0;
    static size_t selectedSprite = 0;
    static float projAngle = 0;
    static float projSpeed = 0;

    ImGui::InputInt("Position X", &posX);
    ImGui::InputInt("Position Y", &posY);

    ImGui::Separator();
     if(ImGui::CollapsingHeader("Projectile", ImGuiTreeNodeFlags_DefaultOpen)){

      ImGui::SliderAngle("Object Angle", &objVelocityAngle, 0 , 360); //NOLINT
      ImGui::SliderFloat("Object Speed", &obVelocitySpeed, 10 , 100); //NOLINT

    }

    ImGui::Spacing();

    auto textures = store->getTextureNames();
    
    if(textures.empty()){
      textures.emplace_back("empty");
    }

    std::string previewValue = textures[selectedSprite];

    if (ImGui::BeginCombo("Textures", previewValue.c_str(), 0)) {

      for (size_t index = 0; index < textures.size(); index++) {
        bool selected = index == selectedSprite;

        if (ImGui::Selectable(textures[index].c_str(), selected)) {
          selectedSprite = index;
        }

        if (selected) {
          ImGui::SetItemDefaultFocus();
        }
      }

      ImGui::EndCombo();
    }

    ImGui::Separator();

    if(ImGui::CollapsingHeader("Projectile", ImGuiTreeNodeFlags_DefaultOpen)){

      ImGui::SliderAngle("Angle", &projAngle, 0 , 360); //NOLINT
      ImGui::SliderFloat("Speed", &projSpeed, 10 , 100); //NOLINT

    }

    ImGui::Separator();

    if (ImGui::Button("Spawn!")) {
      struct TileSize {
        int32_t width = 35;
        int32_t height = 32;
      };

      TileSize tileSize;

      //NOTE: Converting angle / speed into x & y values
      auto projVel = glm::vec2(cos(projAngle) * projSpeed, sin(projAngle) * projSpeed);
      auto objVel = glm::vec2(cos(objVelocityAngle) * obVelocitySpeed, sin(objVelocityAngle) * obVelocitySpeed);

      auto tank = worldManager->createGameObject();
      tank.addComponent<SpriteComponent>( textures[selectedSprite], glm::vec2(tileSize.width, tileSize.height), 2); // NOLINT
      tank.addComponent<TransformComponent>( glm::vec2(posX, posY), glm::vec2(1, 1), rotation);        // NOLINT
      tank.addComponent<RigidBodyComponent>(objVel); // NOLINT
      tank.addComponent<BoxColliderComponent>( glm::vec2(tileSize.width, tileSize.height));
      tank.addComponent<ProjectileEmiterComponent>("bullet", glm::vec2(4, 4), projVel, 1000, 1500, 40); // NOLINT
      tank.addComponent<HealthComponent>();
      tank.joinAlliance(configurables::Alliances::ENEMIES);
    }
  }
  ImGui::End();

  // Coordinates Window window
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav; // NOLINT

  ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always,ImVec2(0,0)); //NOLINT
  ImGui::SetNextWindowBgAlpha(0.5f); // NOLINT

  if(ImGui::Begin("Game Coordinates", nullptr, windowFlags)){

    float posX = ImGui::GetIO().MousePos.x < 0 ? 0 : ImGui::GetIO().MousePos.x  + static_cast<float>(camera.x());
    float posY = ImGui::GetIO().MousePos.y < 0 ? 0 : ImGui::GetIO().MousePos.y + static_cast<float>(camera.y());

    ImGui::Text("Coordinates: x=%.1f, y=%.1f", posX, posY); // NOLINT

  }

  ImGui::End();

  ImGui::Render();
  ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}
