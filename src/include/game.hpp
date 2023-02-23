#ifndef GAME_H
#define GAME_H
#include "renderer.hpp"

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
  void update();
  void render();
  void processInput();
  
  [[nodiscard]] auto isValid() const -> bool;

private:
  bool          m_valid     = true;
  bool          m_isRunning = false; 
  Window        m_window;
  Renderer      m_renderer;

};

#endif
