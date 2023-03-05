#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include <cstdlib>
#include <doctest.h>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

#include "colors.hpp"
#include "components/all.hpp"
#include "game.hpp"
#include "systems/all.hpp"
#include "utils//numbers.hpp"

Game::Game()
    : m_window(std::make_unique<Window>(WINDOW_WIDTH, WINDOW_HEIGHT)),
      m_renderer(std::make_unique<Renderer>(m_window)),
      m_wm(std::make_unique<WorldManager>()),
      m_store(std::make_unique<AssetStore>()),
      m_eventBus(std::make_unique<EventBus>())
{}

Game::~Game() {
  m_window->~Window(); // destroy window first
  m_renderer->~Renderer();
  SDL_Quit();
}

/** **/

void Game::loadLevel(uint32_t level) {

  spdlog::info("loading level {}", level);

  m_store->loadTexture(m_renderer, "tank-right", "./assets/images/tank-panther-right.png");
  m_store->loadTexture(m_renderer, "truck-left", "./assets/images/truck-ford-left.png");
  m_store->loadTexture(m_renderer, "map", "./assets/tilemaps/jungle.png");
  m_store->loadTexture(m_renderer, "chopper", "./assets/images/chopper.png");
  m_store->loadTexture(m_renderer, "radar", "./assets/images/radar.png");

  MapBuilder mapBuilder("./assets/tilemaps/jungle.map", "map", TILE);
  mapBuilder.build(m_wm);

  // possibly getting confused by the params order
  // NOTE: potentially use the builder patterns to avoid passing a bunch of
  // parameters at the same time and

  auto tank = m_wm->createGameObject();
  tank.addComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(1, 1), 0.0);                 // NOLINT
  tank.addComponent<RigidBodyComponent>(glm::vec2(0.2, 0.0)); // NOLINT
                                                              //
  tank.addComponent<SpriteComponent>( "tank-right", glm::vec2(TILE.width, TILE.height),
      SpriteComponent::makeCrop(0, 0, TILE.width, TILE.height), 2); // NOLINT

  tank.addComponent<BoxColliderComponent>(glm::vec2(TILE.width, TILE.height));
  tank.addComponent<DebugComponent>();

  auto truck = m_wm->createGameObject();
  truck.addComponent<TransformComponent>(glm::vec2(200, 0), glm::vec2(1, 1), 0.0);
  truck.addComponent<RigidBodyComponent>(glm::vec2(-0.2, 0.0)); // NOLINT

  truck.addComponent<SpriteComponent>( "truck-left", glm::vec2(TILE.width, TILE.height),
      SpriteComponent::makeCrop(0, 0, TILE.width, TILE.height), 1); // NOLINT

  truck.addComponent<BoxColliderComponent>(glm::vec2(TILE.width, TILE.height));

  truck.addComponent<DebugComponent>();

  auto chopper = m_wm->createGameObject();
  chopper.addComponent<TransformComponent>(glm::vec2(200, 200), glm::vec2(1, 1));     // NOLINT
  chopper.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0)); // NOLINT
  chopper.addComponent<SpriteComponent>( "chopper", glm::vec2(TILE.width, TILE.height),
      SpriteComponent::makeCrop(0, 0, TILE.width, TILE.height), 5);
  // we only hav2 two frames, with a 5 fps
  chopper.addComponent<AnimationComponent>(2, 8); // NOLINT

  auto radar = m_wm->createGameObject();
  radar.addComponent<TransformComponent>(glm::vec2(WINDOW_WIDTH - 74, 10), glm::vec2(1, 1));     // NOLINT
  radar.addComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0)); // NOLINT
  radar.addComponent<SpriteComponent>(
      "radar", glm::vec2(TILE.width * 2, TILE.height * 2),
      SpriteComponent::makeCrop(0, 0, TILE.width * 2, TILE.height * 2), 1);
  // we only hav2 two frames, with a 5 fps
  radar.addComponent<AnimationComponent>(8, 5); // NOLINT
                                                //
}

/** **/

void Game::setup() {
  /* Registering systems in the world on setup */
  m_wm->createSystem<MovementSystem>();
  m_wm->createSystem<RenderSystem>();
  m_wm->createSystem<AnimationSystem>();
  m_wm->createSystem<CollisionSystem>();
  m_wm->createSystem<RenderDebugSystem>();
  m_wm->createSystem<DamageSystem>();
  m_wm->createSystem<KeyboardMovementSystem>();
}

/** **/

void Game::update() {

  /* removing this fuction will allow the game to run as fast as it can! */
  capFrameRate();

  /* subscribe event listeners */
  handleEvents();

  /* add newly created GameObjects to system */
  m_wm->update();

  double delta = deltatime();
  m_wm->getSystem<MovementSystem>().update(delta);
  m_wm->getSystem<AnimationSystem>().update();
  m_wm->getSystem<CollisionSystem>().update(m_eventBus);

  m_prevFrameTime = SDL_GetTicks();
}

void Game::processInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {

    switch (event.type) {
    case SDL_QUIT:
      m_isRunning = false;

    case SDL_KEYDOWN:
      m_eventBus->dispatchEvent<KeyPressEvent>(event.key.keysym.sym, event.key.keysym.mod);

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
  m_renderer->setDrawColor(Gray());
  m_renderer->clear();
  m_wm->getSystem<RenderSystem>().update(m_renderer, m_store);
  m_wm->getSystem<RenderDebugSystem>().update(m_renderer);
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

void Game::handleEvents(){

  //TODO: Do we need to subscribe & clear bus at every frame?
  // We could improve this with a smarter data structure.
  // A buffer of subscriptions that are only added and removed at certain "events" or for a certain object ID.
  // For example, when an entity is removed, then we go ahead and remove all the events
  // associated with that entity.
  m_eventBus->clear();
  m_wm->getSystem<DamageSystem>().addEventListeners(m_eventBus);
  m_wm->getSystem<KeyboardMovementSystem>().addEventListeners(m_eventBus);
}

void Game::capFrameRate() const {
  uint32_t waitTime = MSECS_PER_FRAME - (m_prevFrameTime - SDL_GetTicks());

  if (waitTime > 0 && waitTime < MSECS_PER_FRAME) {
    SDL_Delay(waitTime);
  }
}

auto Game::deltatime() const -> double {
  return (SDL_GetTicks(), m_prevFrameTime) / MILLISECS;
}
