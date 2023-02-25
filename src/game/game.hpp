#ifndef GAME_H
#define GAME_H
#include <memory>
#include "renderer.hpp"
#include "ecs/world_manager.hpp"

class Game {

public:
  Game();
  ~Game();

  Game(Game &) = delete;
  Game(Game &&) = delete;

  void operator=(Game &) = delete;
  void operator=(Game &&) = delete;

  void setup();
  void run();
  void update() const;
  void render();
  void processInput();
  
  [[nodiscard]] auto isValid() const -> bool;

private:
  bool          m_isRunning = false; 
  uint32_t      m_prevFrameTime = 0; 
  Window        m_window;
  Renderer      m_renderer;
  std::unique_ptr<WorldManager> m_wm;

  void capFrameRate()const;

  constexpr static const int8_t FPS = 60; 
  constexpr static const int8_t MSECS_PER_FRAME = 1000/FPS; 
  constexpr static const double MILLISECS =  1000.0;
};

#endif
