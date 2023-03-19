#ifndef APPLICATION_H
#define APPLICATION_H

#include "game/game_engine.hpp"

class Application {

  public:
    Application();
    ~Application();

  Application(Application &) = delete;
  Application(Application &&) = delete;

  void operator=(Application &) = delete;
  void operator=(Application &&) = delete;


    void run();

  private:
    GameEngine m_engine;
    sol::state m_lua;
};

#endif
