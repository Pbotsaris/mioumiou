#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>
#include <glm.hpp>
#include "window.hpp"
#include "colors.hpp"

struct ImageDimensions {
  const SDL_Rect *sourceCrop;
  const SDL_Rect *destDimension;
  const double rotation;
  ImageDimensions(const SDL_Rect *crop, const SDL_Rect *dimension, const double rotation);
};

struct TextDimensions {
  const int32_t x;
  const int32_t y;
};

class Renderer {

public:
  explicit Renderer(std::unique_ptr<Window> &window);
  ~Renderer();

  Renderer(Renderer &) = delete;
  Renderer(Renderer &&) = delete;

  Renderer& operator=(const Renderer&) = delete;
  Renderer& operator=(Renderer &&) = delete;


  [[nodiscard]] auto sdlRenderer() const -> SDL_Renderer*;

  void setDrawColor(Colors &&color);
  void fillRect(SDL_Rect *rect);
  void drawRect(SDL_Rect *rect);
  void clear();
  void present();
  void drawImage(SDL_Texture *tex, ImageDimensions &&dimensions);

  /* return the actual dimensions of rendered text */
  auto drawText(SDL_Texture *tex, TextDimensions &&dimensions) -> glm::vec2;
  [[nodiscard]] auto createTexture(const std::string &path) const -> SDL_Texture*;
  [[nodiscard]] auto createTextureFromSurface(SDL_Surface *surface) const -> SDL_Texture*;

  [[nodiscard]] auto valid() const -> bool;


private:
  bool          m_valid;
  SDL_Renderer *m_renderer = nullptr;

};

#endif
