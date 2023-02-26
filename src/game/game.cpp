#include <SDL2/SDL.h>
#include <iostream>
#include <spdlog/spdlog.h>

#include "game.hpp"
#include "colors.hpp"
#include "components/all.hpp"
#include "systems/all.hpp"

Game::Game()
    : m_renderer(std::make_unique<Renderer>(m_window)), m_wm(std::make_unique<WorldManager>()) {
  if (m_renderer->valid()) {
    m_window.setFullScreen();
  }
}

Game::~Game() {
  m_window.~Window(); // destroy window first
  m_renderer->~Renderer();
  SDL_Quit();
}


void Game::setup() {
  auto tank = m_wm->createGameObject();
  tank.addComponent<TransformComponent>(glm::vec2(0,0), glm::vec2(1,1), 1);
  tank.addComponent<RigidBodyComponent>(glm::vec2(1,1));
  tank.addComponent<SpriteComponent>("./assets/images/tank-panther-right.png", glm::vec2(32, 32));  //NOLINT

  /* Registering systems in the world on setup */
  m_wm->createSystem<MovementSystem>();
  m_wm->createSystem<RenderSystem>();
}

void Game::update() {

  /* removing this fuction will allow the game to run as fast as it can! */
  capFrameRate();

  double delta = deltatime();
  m_wm->getSystem<MovementSystem>().update(delta); // will update the system at every iteration
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
    m_wm->getSystem<RenderSystem>().update(m_renderer);
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
  return  (SDL_GetTicks(), m_prevFrameTime) / MILLISECS;
}
