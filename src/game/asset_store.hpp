#ifndef ASSET_STORE_H
#define ASSET_STORE_H

#include <unordered_map>
#include "renderer.hpp"
#include <memory>
#include <string>
#include <SDL2/SDL.h>

class AssetStore {
  public:
  AssetStore() = default;
  ~AssetStore();

  // no copying or moving the asset store
  AssetStore(AssetStore &) = delete;
  AssetStore(AssetStore &&) = delete;

  AssetStore& operator=(const AssetStore&) = delete;
  AssetStore& operator=(AssetStore&&) = delete;


  void loadTexture(std::unique_ptr<Renderer> &renderer, std::string &&key, const std::string &path);
  auto getTexture(const std::string &key) const -> SDL_Texture*;
  void removeTexture(const std::string &key);
  void clear();

  private:
    std::unordered_map<std::string, SDL_Texture*> m_textures;

  void cleanup();
};


#endif
