#include "renderer.hpp"
#include "SDL_surface.h"

Renderer::Renderer(Window &window)
    : m_valid(true),
      m_renderer(SDL_CreateRenderer(window.sdlWindow(), -1,
                                    SDL_RENDERER_ACCELERATED |
                                        SDL_RENDERER_PRESENTVSYNC)) {
  m_valid = m_renderer != nullptr && window.valid();
}

Renderer::~Renderer(){
  SDL_DestroyRenderer(m_renderer);
}

auto Renderer::valid() const -> bool { return m_valid; }

void Renderer::setDrawColor(Colors &&color) {
  RGBA rgba = color.getColor();
  SDL_SetRenderDrawColor(m_renderer, rgba.r, rgba.g, rgba.b, rgba.a); // black
}

void Renderer::clear() {SDL_RenderClear(m_renderer);}
void Renderer::present() {SDL_RenderPresent(m_renderer);}
void Renderer::fillReact(SDL_Rect *rect) {SDL_RenderFillRect(m_renderer, rect) ;}

