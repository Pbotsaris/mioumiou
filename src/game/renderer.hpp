#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include <string>
#include <memory>
#include "window.hpp"
#include "colors.hpp"

struct Destination {

  Destination(int32_t x, int32_t y, int32_t w, int32_t h); // NOLINT

  SDL_Rect react;
};

class Renderer {

public:
  explicit Renderer(std::unique_ptr<Window> &window);
  ~Renderer();

  Renderer(Renderer &) = delete;
  Renderer(Renderer &&) = delete;

  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer &&) = delete;

  void setDrawColor(Colors &&color);
  void fillReact(SDL_Rect *rect);
  void clear();
  void present();
  void drawImage(SDL_Texture *tex, Destination &&dest);
  auto createTexture(const std::string &path)-> SDL_Texture*;

  [[nodiscard]] auto valid() const -> bool;


private:
  bool          m_valid;
  SDL_Renderer *m_renderer = nullptr;

};

#endif
