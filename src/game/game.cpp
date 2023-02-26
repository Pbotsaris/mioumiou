#include <SDL2/SDL.h>
#include <iostream>
#include <spdlog/spdlog.h>

#include "colors.hpp"
#include "components/all.hpp"
#include "game.hpp"
#include "systems/all.hpp"

Game::Game()
    : m_window(std::make_unique<Window>()),
      m_renderer(std::make_unique<Renderer>(m_window)),
      m_wm(std::make_unique<WorldManager>()),
      m_store(std::make_unique<AssetStore>()) {
  if (m_renderer->valid()) {
    m_window->setFullScreen();
  }
}

Game::~Game() {
  m_window->~Window(); // destroy window first
  m_renderer->~Renderer();
  SDL_Quit();
}

void Game::setup() {

  m_store->loadTexture(m_renderer, "tank-right",
                       "./assets/images/tank-panther-right.png");
  m_store->loadTexture(m_renderer, "tree", "./assets/images/tree.png");

  auto tank = m_wm->createGameObject();
  tank.addComponent<TransformComponent>(glm::vec2(0, 0), glm::vec2(1, 1), 0.0);
  tank.addComponent<RigidBodyComponent>(glm::vec2(1, 1));
  tank.addComponent<SpriteComponent>("tank-right", glm::vec2(32, 32), SpriteComponent::makeCrop(0, 0, 32, 32)); // NOLINT

  auto tank2 = m_wm->createGameObject();
  tank2.addComponent<TransformComponent>(glm::vec2(0, 40), glm::vec2(1, 1), 0.0); // NOLINT
  tank2.addComponent<RigidBodyComponent>(glm::vec2(1, 0));

  tank2.addComponent<SpriteComponent>("tree", glm::vec2(23, 23), SpriteComponent::makeCrop(0, 0, 32, 32)); // NOLINT

  /* Registering systems in the world on setup */
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

  setup();
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
