#ifndef GAME_H
#define GAME_H
#include <memory>
#include "game/asset_store.hpp"
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
  void update();
  void render();
  void processInput();

  [[nodiscard]] auto isValid() const -> bool;

private:
  bool          m_isRunning = false; 
  uint32_t      m_prevFrameTime = 0; 
  Window        m_window;
  std::unique_ptr<Renderer>     m_renderer;
  std::unique_ptr<WorldManager> m_wm;
  std::unique_ptr<AssetStore>   m_store;

  void capFrameRate()const;
  [[nodiscard]] auto deltatime() const -> double;

  constexpr static const int8_t FPS = 60; 
  constexpr static const int8_t MSECS_PER_FRAME = 1000/FPS; 
  constexpr static const double MILLISECS =  1000.0;
};

#endif
