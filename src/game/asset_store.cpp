#include "asset_store.hpp"
#include <SDL2/SDL_image.h>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

void AssetStore::loadTexture(std::unique_ptr<Renderer> &renderer, std::string &&key, const std::string &path) {

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


auto AssetStore::getTexture(const std::string &key) -> SDL_Texture* {

   if (m_textures.contains(key)){
     return m_textures[key];
   }

  return nullptr;
};


AssetStore::~AssetStore(){
  // clean up!
  for(auto &texture : m_textures){
    SDL_DestroyTexture(texture.second);
  }
}
