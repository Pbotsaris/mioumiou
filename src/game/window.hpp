#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>

class Window {

public:
  explicit Window(size_t width, size_t height);
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
};

#endif
