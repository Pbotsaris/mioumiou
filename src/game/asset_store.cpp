#include <SDL2/SDL_image.h>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

#include "asset_store.hpp"

void AssetStore::loadTexture(std::unique_ptr<Renderer> &renderer,
                             std::string &&key, const std::string &path) {

  SDL_Texture *tex = renderer->createTexture(path);

  if (tex == nullptr) {
    spdlog::error("Failed to load texture id '{}' from '{}'", key, path);
    return;
  }
  bool ok = m_textures.insert({key, tex}).second; // NOLINT: short var

  if (!ok) {
    spdlog::error( "Failed insert texture id '{}' to Textures map. Destroying texture...", key);
    SDL_DestroyTexture(tex);
  }
}

auto AssetStore::getTexture(const std::string &key) const -> SDL_Texture * {

  if (m_textures.contains(key)) {
    return m_textures.at(key);
  }

  return nullptr;
};

void AssetStore::removeTexture(const std::string &key) {

  if (m_textures.contains(key)) {
    auto *tex = m_textures[key];
    SDL_DestroyTexture(tex);
    m_textures.erase(key);
    return;
  }
  spdlog::error("Failed to remove texture id '{}' to Textures map", key);
}

void AssetStore::clear() {
  cleanup();
  m_textures.clear();
}

AssetStore::~AssetStore() { cleanup(); }

/* Private */

void AssetStore::cleanup() {
  for (auto &texture : m_textures) {
    SDL_DestroyTexture(texture.second);
  }
}
