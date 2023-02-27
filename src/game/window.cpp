#include "window.hpp"

Window::Window(size_t width, size_t height)
    : m_valid(true), m_window(SDL_CreateWindow(nullptr,
                                               SDL_WINDOWPOS_CENTERED, // NOLINT
                                               SDL_WINDOWPOS_CENTERED, // NOLINT
                                               static_cast<int>(width),
                                               static_cast<int>(height),
                                               SDL_WINDOW_BORDERLESS)) {
  m_valid = m_window != nullptr;
}

Window::~Window(){
  SDL_DestroyWindow(m_window);
}

auto Window::sdlWindow() -> SDL_Window * { return m_window; }
auto Window::valid() const -> bool { return m_valid; }

void Window::setFullScreen() {
  // TODO: Error logging when not valid
  if (m_valid) {
    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
  }
}
