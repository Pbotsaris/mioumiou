#include <SDL2/SDL.h>
#include <cstdlib>
#include <doctest.h>
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

#include "colors.hpp"
#include "game.hpp"
#include "utils//numbers.hpp"
#include "components/all.hpp"
#include "systems/all.hpp"

Game::Game()
    : m_window(std::make_unique<Window>(WINDOW_WIDTH, WINDOW_HEIGHT)),
      m_renderer(std::make_unique<Renderer>(m_window)),
      m_wm(std::make_unique<WorldManager>()),
      m_store(std::make_unique<AssetStore>()) {}

Game::~Game() {
  m_window->~Window(); // destroy window first
  m_renderer->~Renderer();
  SDL_Quit();
}

void Game::loadLevel(uint32_t level) {

  spdlog::info("loading level {}", level);

  m_store->loadTexture(m_renderer, "tank-right", "./assets/images/tank-panther-right.png");
  m_store->loadTexture(m_renderer, "tree", "./assets/images/tree.png");
  m_store->loadTexture(m_renderer, "map", "./assets/tilemaps/jungle.png");

  MapBuilder mapBuilder("./assets/tilemaps/jungle.map", "map", TILE);
  mapBuilder.build(m_wm);

  auto tank = m_wm->createGameObject();
  tank.addComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(1, 1), 45);  // NOLINT
  tank.addComponent<RigidBodyComponent>(glm::vec2(0.3, 0.3)); // NOLINT
                                                              //
  tank.addComponent<SpriteComponent>( "tank-right",
      glm::vec2(TILE.width, TILE.height),
      SpriteComponent::makeCrop(0, 0, TILE.width, TILE.height), 2); // NOLINT

  auto tank2 = m_wm->createGameObject();
  tank2.addComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(1, 1), 0.0);       
  tank2.addComponent<RigidBodyComponent>(glm::vec2(0.3, 0.3)); // NOLINT
                                                            
  tank2.addComponent<SpriteComponent>( "tree",
      glm::vec2(TILE.width, TILE.height),
      SpriteComponent::makeCrop(0, 0, TILE.width, TILE.height), 1); // NOLINT
}

void Game::setup() {

  /* Registering systems in the world on setup */
  loadLevel(1);

  m_wm->createSystem<MovementSystem>();
  m_wm->createSystem<RenderSystem>();
}

void Game::update() {

  /* removing this fuction will allow the game to run as fast as it can! */
  capFrameRate();

  double delta = deltatime();
  m_wm->getSystem<MovementSystem>().update(
      delta); // will update the system at every iteration
  m_wm->update();

  m_prevFrameTime = SDL_GetTicks();
}

void Game::processInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event) != 0) {

    switch (event.type) {
    case SDL_KEYDOWN:
    case SDL_QUIT:
      m_isRunning = false;
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        m_isRunning = false;
      }
    }
  }
}

void Game::render() {
  m_renderer->setDrawColor(Gray());
  m_renderer->clear();
  m_wm->getSystem<RenderSystem>().update(m_renderer, m_store);
  m_renderer->present();
}

void Game::run() {
  if (!m_renderer->valid()) {
    return;
  }

  m_window->setFullScreen();
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

void Game::capFrameRate() const {
  uint32_t waitTime = MSECS_PER_FRAME - (m_prevFrameTime - SDL_GetTicks());

  if (waitTime > 0 && waitTime < MSECS_PER_FRAME) {
    SDL_Delay(waitTime);
  }
}

auto Game::deltatime() const -> double {
  return (SDL_GetTicks(), m_prevFrameTime) / MILLISECS;
}

