#include "renderer.hpp"
#include "errors.hpp"
#include <SDL2/SDL_image.h>

ImageDimensions::ImageDimensions(const SDL_Rect *crop, const SDL_Rect *dimension, const double rotation) //NOLINT
  : sourceCrop(crop), destDimension(dimension), rotation(rotation){}; //NOLINT

Renderer::Renderer(std::unique_ptr<Window> &window)
    : m_valid(true),
      m_renderer(SDL_CreateRenderer(window->sdlWindow(), -1,
                                    SDL_RENDERER_ACCELERATED |
                                        SDL_RENDERER_PRESENTVSYNC)) {
  m_valid = m_renderer != nullptr && window->valid();
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

void Renderer::drawImage(SDL_Texture *tex, ImageDimensions &&dimensions){
  SDL_RenderCopyEx(m_renderer, tex, dimensions.sourceCrop, dimensions.destDimension, dimensions.rotation, nullptr, SDL_FLIP_NONE);
}


auto Renderer::createTexture(const std::string &path)-> SDL_Texture*{
  SDL_Surface *surface = IMG_Load(path.c_str());

  if (surface == nullptr) {
    return nullptr;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(m_renderer, surface);
  SDL_FreeSurface(surface);

  return tex;
}
