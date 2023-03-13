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

void Game::loadLevel(uint32_t level) {

  spdlog::info("loading level {}", level);

  m_store->loadTexture(m_renderer, "tank-right", "./assets/images/tank-panther-right.png");
  m_store->loadTexture(m_renderer, "truck-left", "./assets/images/truck-ford-left.png");
  m_store->loadTexture(m_renderer, "map", "./assets/tilemaps/jungle.png");
  m_store->loadTexture(m_renderer, "chopper", "./assets/images/chopper-spritesheet.png");
  m_store->loadTexture(m_renderer, "radar", "./assets/images/radar.png");
  m_store->loadTexture(m_renderer, "bullet", "./assets/images/bullet.png");

  // TODO: Add fonts will all sizes and allow to easily query the store
  m_store ->loadFont("charriot", "./assets/fonts/charriot.ttf");


  TileDimension tileDim = {
      .width = Map::TileDimension::WIDTH,
      .height = Map::TileDimension::HEIGHT,
      .scale = Map::TileDimension::SCALE
      };

  MapBuilder mapBuilder("./assets/tilemaps/jungle.map", "map", tileDim);

  mapBuilder.build(m_wm);

  // possibly getting confused by the params order
  // TODO: potentially use the builder patterns to avoid passing a bunch of
  // parameters at the same time and

  struct TileSize {
    int32_t width = Map::TileDimension::WIDTH;
    int32_t height = Map::TileDimension::HEIGHT;
  };

  TileSize tileSize;

  auto tank = m_wm->createGameObject();
  tank.addComponent<SpriteComponent>( "tank-right", glm::vec2(tileSize.width, tileSize.height), 2); // NOLINT
  tank.addComponent<TransformComponent>(glm::vec2(10, 10), glm::vec2(1, 1), 0.0);                 // NOLINT
  tank.addComponent<RigidBodyComponent>(glm::vec2(0, 0.0)); // NOLINT
  tank.addComponent<BoxColliderComponent>(glm::vec2(tileSize.width, tileSize.height));
  tank.addComponent<DebugComponent>();
  tank.addComponent<ProjectileEmiterComponent>("bullet", glm::vec2(4, 4), glm::vec2(40, 40), 1000, 1500, 40); // NOLINT
  tank.addComponent<HealthComponent>();                                                                                             
  tank.joinAlliance(configurables::Alliances::ENEMIES);

  // TODO: This is necessary to compile? why?
 // auto comps = tank.getComponent<HealthComponent>();
 // spdlog::warn(comps.maxHealth);
 // spdlog::warn(tank.hasComponent<HealthComponent>());

  auto truck = m_wm->createGameObject();
  truck.addComponent<SpriteComponent>("truck-left", glm::vec2(tileSize.width, tileSize.height), 1); // NOLINT
  truck.addComponent<TransformComponent>(glm::vec2(200, 0), glm::vec2(1, 1), 0.0); //NOLINT
  truck.addComponent<RigidBodyComponent>(glm::vec2(20, 0.0)); // NOLINT
  truck.addComponent<BoxColliderComponent>(glm::vec2(tileSize.width, tileSize.height));
  truck.addComponent<DebugComponent>();
  truck.addComponent<HealthComponent>();
  truck.addComponent<HealthBarComponent>();
  truck.addComponent<HealthBarTextComponent>();
  tank.joinAlliance(configurables::Alliances::ENEMIES);

  // ** //
  auto chopper = m_wm->createGameObject();
  chopper.addComponent<SpriteComponent>( "chopper", glm::vec2(tileSize.width, tileSize.height), 5); //NOLINT
  chopper.addComponent<TransformComponent>(glm::vec2(200, 200), glm::vec2(1, 1));     // NOLINT
  chopper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0)); // NOLINT
  chopper.addComponent<AnimationComponent>(2, 8); // NOLINT
  chopper.addComponent<KeyboardControlComponent>(80, 80, 80, 80); // NOLINT
  chopper.addComponent<CameraFollowerComponent>();
  chopper.addComponent<BoxColliderComponent>(glm::vec2(tileSize.width, tileSize.height));
  chopper.addComponent<ProjectileEmiterComponent>("bullet", glm::vec2(4, 4), glm::vec2(40, 40), 0, 5000); // NOLINT
  chopper.addComponent<ProjectileControlComponent>();
  chopper.addComponent<HealthComponent>(); 
  chopper.addComponent<HealthBarComponent>(constants::Defaults::HealthBar::SIZE, glm::vec2(0,0), HealthBarComponent::BarAlign::Left);
  chopper.addComponent<HealthBarTextComponent>();
  chopper.joinAlliance(configurables::Alliances::PLAYER);
  chopper.joinAlliance(configurables::Alliances::NEUTRAL);

  // ** //
  auto radar = m_wm->createGameObject();
  radar.addComponent<SpriteComponent>( "radar", glm::vec2(tileSize.width * 2, tileSize.height * 2), 1, true);
  radar.addComponent<TransformComponent>(glm::vec2(Resolution::WINDOW_WIDTH - 74, 10), glm::vec2(1, 1));     // NOLINT
  radar.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0)); // NOLINT
  // we only hav2 two frame, with a 5 fps
  radar.addComponent<AnimationComponent>(8, 5); // NOLINT
                                                
  auto label = m_wm->createGameObject();
  label.addComponent<TextComponent>("hello, world! How are you", "charriot", constants::Fonts::Small, glm::vec2(100, 100), Red()); //NOLINT
}

/** **/

void Game::setup() {
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

  // m_window->setFullScreen();
  setup();
  loadLevel(1);
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
