#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>

class Window {

public:
  Window();
  ~Window();

  Window(Window &) = delete;
  Window(Window &&) = delete;

  void operator=(Window &) = delete;
  void operator=(Window &&) = delete;

  auto sdlWindow()-> SDL_Window*;
  [[nodiscard]] auto valid() const -> bool;
  void setFullScreen();

private:
  bool        m_valid;
  SDL_Window *m_window = nullptr;

  static const size_t WINDOW_WIDTH = 800;
  static const size_t WINDOW_HEIGHT = 600;
};

#endif
