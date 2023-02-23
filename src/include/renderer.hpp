#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include "window.hpp"
#include "colors.hpp"

struct Destination {

  Destination(int32_t x, int32_t y, int32_t w, int32_t h); // NOLINT

  SDL_Rect react;
};

class Renderer {

public:
  explicit Renderer(Window &window);
  ~Renderer();

  Renderer(Renderer &) = delete;
  Renderer(Renderer &&) = delete;

  void operator=(Renderer &) = delete;
  void operator=(Renderer &&) = delete;

  void setDrawColor(Colors &&color);
  void fillReact(SDL_Rect *rect);
  void clear();
  void present();
  auto drawImage(const char* path, Destination &&dest) -> bool;
  [[nodiscard]] auto valid() const -> bool;


private:
  bool          m_valid;
  SDL_Renderer *m_renderer = nullptr;

};

#endif
