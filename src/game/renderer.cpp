#include "renderer.hpp"
#include "errors.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <spdlog/spdlog.h>

ImageDimensions::ImageDimensions(
    const SDL_Rect *crop, // NOLINT
    const SDL_Rect *dimension, 
    const double rotation,
    SDL_RendererFlip flip
    ) 
  : sourceCrop(crop), destDimension(dimension), rotation(rotation), flip(flip){}; //NOLINT

Renderer::Renderer(std::unique_ptr<Window> &window)
    : m_valid(true),
      m_renderer(SDL_CreateRenderer(window->sdlWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)) {
  m_valid = m_renderer != nullptr && window->valid();
}

Renderer::~Renderer() { SDL_DestroyRenderer(m_renderer); }
auto Renderer::valid() const -> bool { return m_valid; }


auto Renderer::sdlRenderer() const -> SDL_Renderer*{
  return m_renderer;
}

void Renderer::setDrawColor(Colors &&color) {
  auto rgba = color.getColor();
  SDL_SetRenderDrawColor(m_renderer, rgba.r, rgba.g, rgba.b, rgba.a); 
}

void Renderer::clear() { SDL_RenderClear(m_renderer); }
void Renderer::present() { SDL_RenderPresent(m_renderer); }
void Renderer::fillRect(SDL_Rect *rect) { SDL_RenderFillRect(m_renderer, rect); }
void Renderer::drawRect(SDL_Rect *rect) { SDL_RenderDrawRect(m_renderer, rect); }

void Renderer::drawImage(SDL_Texture *tex, ImageDimensions &&dimensions){
  int32_t res = SDL_RenderCopyEx(m_renderer,
                                tex,
                                dimensions.sourceCrop,
                                dimensions.destDimension,
                                dimensions.rotation,
                                nullptr,
                                dimensions.flip);

  if(res != 0){
    spdlog::warn("Could not render Image. Error: '{}'", SDL_GetError());
  }

}

auto Renderer::drawText(SDL_Texture *tex, TextDimensions &&dimensions) -> glm::vec2 {
  int32_t width = 0;
  int32_t height = 0;

  if(SDL_QueryTexture(tex, nullptr, nullptr, &width, &height) != 0){
    spdlog::warn("Could not query Text texture. Error: '{}'", SDL_GetError());
  }

  SDL_Rect destination {
    .x = dimensions.x,
    .y = dimensions.y,
    .w = width,
    .h = height
  };


  if(SDL_RenderCopy(m_renderer, tex, nullptr, &destination) != 0){
    spdlog::warn("Could not render Text. Error: '{}'", SDL_GetError());
  }

  // must clean up texture as we have the fonts in store not the actual textures.
  SDL_DestroyTexture(tex);
  return {width, height};
}


auto Renderer::createTexture(const std::string &path) const -> SDL_Texture*{
  SDL_Surface *surface = IMG_Load(path.c_str());

  if (surface == nullptr) {
    spdlog::error("Failed to load Texture from Path.");
    return nullptr;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(m_renderer, surface);
  SDL_FreeSurface(surface);

  return tex;
}
auto Renderer::createTextureFromSurface(SDL_Surface *surface) const -> SDL_Texture* {

  if(surface == nullptr){
    spdlog::error("Received a nullptr surface. Could not create texture. Error: '{}'", SDL_GetError());
  }


  SDL_Texture *tex = SDL_CreateTextureFromSurface(m_renderer,  surface);

  SDL_FreeSurface(surface);

  if(tex == nullptr){
    spdlog::error("Failed to load Texture from Surface.");
  }

  return tex;
}
