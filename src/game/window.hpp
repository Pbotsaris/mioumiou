#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>

class Window {

public:
  Window();
  ~Window();

  Window(Window &) = delete;
  Window(Window &&) = delete;

  Window& operator=(const Window &) = delete;
  Window& operator=(Window &&) = delete;

  auto sdlWindow()-> SDL_Window*;
  [[nodiscard]] auto valid() const -> bool;
  void setFullScreen();

private:
  bool        m_valid;
  SDL_Window *m_window = nullptr;
  static const size_t WINDOW_WIDTH = 1920;
  static const size_t WINDOW_HEIGHT = 1080;
};

#endif
