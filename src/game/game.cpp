#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <cstdlib>
#include <doctest.h>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer.h>

#include "colors.hpp"
#include "components/all.hpp"
#include "game.hpp"
#include "systems/all.hpp"
#include "utils/numbers.hpp"
#include "utils/configurables.hpp"
#include "level_manager.hpp"


using namespace configurables;

Game::Game()
    : m_window(std::make_unique<Window>(Resolution::WINDOW_WIDTH, Resolution::WINDOW_HEIGHT)),
      m_renderer(std::make_unique<Renderer>(m_window)),
      m_wm(std::make_unique<WorldManager>()),
      m_store(std::make_unique<AssetStore>()),
      m_eventBus(std::make_unique<EventBus>()),
      m_camera(Camera::Position(0, 0), Camera::Dimension(Resolution::WINDOW_WIDTH, Resolution::WINDOW_HEIGHT)) { 

   /* SDL Init */
   if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
     spdlog::critical("Could not initialize SDL. Error: '{}'", SDL_GetError());
   }
  
   if(TTF_Init() != 0){
     spdlog::critical("Could not initialize TTF. Error: '{}'", TTF_GetError());
   }

   /* Dear ImGui Init */
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGui::StyleColorsDark();

   /* NOTE: no key for now */
   // ImGuiIO& io{ImGui::GetIO()}; //NOLINT
   // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // NOLINT
   if(!ImGui_ImplSDL2_InitForSDLRenderer(m_window->sdlWindow(), m_renderer->sdlRenderer())){
     spdlog::critical("Failed to initialize ImGui with SDL");
   }

   if(!ImGui_ImplSDLRenderer_Init(m_renderer->sdlRenderer())){
     spdlog::critical("Failed to initialize ImGui with SDL Renderer.");
   }

   /* Lua Init */
   m_lua.open_libraries(sol::lib::base, sol::lib::math);
}

Game::~Game() {
  m_window->~Window(); // IMPORTANT: destroy window first
  m_renderer->~Renderer();
  SDL_Quit();
  ImGui_ImplSDLRenderer_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  //TTF_Quit(); -> NOTE: Why segfaults?
}


/** **/

void Game::setup() {

  // m_window->setFullScreen();

  /* Registering systems in the world on setup */
  m_wm->createSystem<MovementSystem>();
  m_wm->createSystem<RenderSpriteSystem>();
  m_wm->createSystem<AnimationSystem>();
  m_wm->createSystem<CollisionSystem>();
  m_wm->createSystem<RenderDebugSystem>();
  m_wm->createSystem<DamageSystem>();
  m_wm->createSystem<KeyboardControlSystem>();
  m_wm->createSystem<CameraMovementSystem>();
  m_wm->createSystem<ProjectileEmitSystem>();
  m_wm->createSystem<ProjectileControlSystem>();
  m_wm->createSystem<ProjectileLifeCycleSystem>();
  m_wm->createSystem<RenderTextSystem>();
  m_wm->createSystem<RenderHealthBarSystem>();
  m_wm->createSystem<RenderGuiSystem>();

  LevelManager levelManager(m_lua, "./assets/scripts/level_1.lua", 1);
  levelManager.loadAssets(m_store, m_renderer);
  levelManager.loadMap(m_wm);
  levelManager.loadGameObjects(m_wm);
}

/** **/

void Game::update() {
  /* NOTE: Maybe Render and update should run a separated threads? Must figure out game */

  /* removing this fuction will allow the game to run as fast as it can! */
  capFrameRate();

  /* subscribe event listeners */
  addEventListeners();

  /* add newly created GameObjects to system */
  m_wm->update();

  double delta = deltatime();
  m_wm->getSystem<CameraMovementSystem>().update(m_camera);
  m_wm->getSystem<MovementSystem>().update(delta);
  m_wm->getSystem<AnimationSystem>().update();
  m_wm->getSystem<CollisionSystem>().update(m_eventBus);
  m_wm->getSystem<ProjectileEmitSystem>().update();
  m_wm->getSystem<ProjectileLifeCycleSystem>(). update();

  m_prevFrameTime = SDL_GetTicks();
}

void Game::processInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {

    //TODO: Make a class for SDL_Events 
     ImGui_ImplSDL2_ProcessEvent(&event);
     ImGuiIO& io{ImGui::GetIO()}; //NOLINT

     int32_t mouseX = 0;
     int32_t mouseY = 0;

     const uint32_t buttons = SDL_GetMouseState(&mouseX, &mouseY);
     io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
     io.MouseDown[0] = static_cast<bool>(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)); //NOLINT: Bad bitwise warning
     io.MouseDown[1] = static_cast<bool>(buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)); //NOLINT: Bad bitwise warning

    switch (event.type) {
    case SDL_QUIT:
      m_isRunning = false;

    case SDL_KEYDOWN:
      m_eventBus->dispatchEvent<KeyDownEvent>(event.key.keysym.sym, event.key.keysym.mod);

      if (event.key.keysym.sym == SDLK_ESCAPE) {
        m_isRunning = false;
      }
      if (event.key.keysym.sym == SDLK_d) {
        auto renderDebug = m_wm->getSystem<RenderDebugSystem>();

        for (auto &objs : renderDebug.gameObjects()) {
          auto &debug = objs.getComponent<DebugComponent>();
          debug.collision.isVisible = !debug.collision.isVisible;
        }
      }
    }
  }
}

void Game::render() {
  m_renderer->clear();

  /* NOTE: Order maters here */
  m_wm->getSystem<RenderSpriteSystem>().update(m_renderer, m_camera, m_store);
  m_wm->getSystem<RenderDebugSystem>().update(m_renderer, m_camera);
  m_wm->getSystem<RenderTextSystem>().update(m_renderer, m_store, m_camera);
  m_wm->getSystem<RenderHealthBarSystem>().update(m_renderer, m_store, m_camera);
  m_wm->getSystem<RenderGuiSystem>().update(m_wm, m_store, m_camera);

  m_renderer->present();
}

void Game::run() {

  if (!m_renderer->valid()) {
    return;
  }

  setup();
  m_isRunning = true;

  while (m_isRunning) {
    processInput();
    update();
    render();
  }
}

/* Private */

void Game::addEventListeners(){

  //TODO: Do we need to subscribe & clear bus at every frame? Maybe run events on another thread?
  // We could improve this with a smarter data structure.
  // A buffer of subscriptions that are only added and removed at certain "events" or for a certain object ID.
  // For example, when an entity is removed, then we go ahead and remove all the events
  // associated with that entity.
  m_eventBus->clear();
  m_wm->getSystem<DamageSystem>().addEventListeners(m_eventBus);
  m_wm->getSystem<KeyboardControlSystem>().addEventListeners(m_eventBus);
  m_wm->getSystem<ProjectileControlSystem>().addEventListeners(m_eventBus);
  m_wm->getSystem<MovementSystem>().addEventListeners(m_eventBus);
}

void Game::capFrameRate() const {
  uint32_t waitTime = MSECS_PER_FRAME - (m_prevFrameTime - SDL_GetTicks());

  if (waitTime > 0 && waitTime < MSECS_PER_FRAME) {
    SDL_Delay(waitTime);
  }
}

auto Game::deltatime() const -> double {
  return (SDL_GetTicks() - m_prevFrameTime) / constants::Time::MILLIS_IN_SEC;
}
