#include "include/game.hpp"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "colors.hpp"

Game::Game() : m_renderer(m_window) {
  m_valid = m_renderer.valid();

  if (m_valid) {
    m_window.setFullScreen();
  }
}

Game::~Game() {
  m_window.~Window(); // destroy window first
  m_renderer.~Renderer();
  SDL_Quit();
}

void Game::setup() {}

void Game::update() {}

auto Game::isValid() const -> bool { return m_valid; }

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

  m_renderer.setDrawColor(Gray());
  m_renderer.clear();

  SDL_Rect player = {10, 10, 20, 20};
  m_renderer.setDrawColor(White());
  m_renderer.fillReact(&player);
  m_renderer.present();
}

void Game::run() {

  if (!isValid()) {
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
