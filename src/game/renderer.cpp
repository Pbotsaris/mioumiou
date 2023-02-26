#include "renderer.hpp"
#include "errors.hpp"
#include <SDL2/SDL_image.h>

Destination::Destination(int32_t x, int32_t y, int32_t w, int32_t h) : react({x, y, w, h}){} // NOLINT

Renderer::Renderer(Window &window)
    : m_valid(true),
      m_renderer(SDL_CreateRenderer(window.sdlWindow(), -1,
                                    SDL_RENDERER_ACCELERATED |
                                        SDL_RENDERER_PRESENTVSYNC)) {
  m_valid = m_renderer != nullptr && window.valid();
}

Renderer::~Renderer() { SDL_DestroyRenderer(m_renderer); }

auto Renderer::valid() const -> bool { return m_valid; }

void Renderer::setDrawColor(Colors &&color) {
  auto rgba = color.getColor();
  SDL_SetRenderDrawColor(m_renderer, rgba.r, rgba.g, rgba.b, rgba.a); // black
}

void Renderer::clear() { SDL_RenderClear(m_renderer); }
void Renderer::present() { SDL_RenderPresent(m_renderer); }
void Renderer::fillReact(SDL_Rect *rect) {
  SDL_RenderFillRect(m_renderer, rect);
}

auto Renderer::drawImage(const std::string &path, Destination &&dest) -> bool {
  SDL_Surface *surface = IMG_Load(path.c_str());

  if (surface == nullptr) {
    return false;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(m_renderer, surface);

  if(tex == nullptr){
    return false;
  }

  SDL_FreeSurface(surface);
  SDL_RenderCopy(m_renderer, tex, nullptr, &dest.react);
  SDL_DestroyTexture(tex);

  return true;
}
