#ifndef ASSET_STORE_H
#define ASSET_STORE_H

#include <unordered_map>
#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <array>
#include <vector>

#include "types/types.hpp"
#include "renderer.hpp"

class AssetStore {
  public:
  AssetStore() = default;
  ~AssetStore();


  // no copying or moving the asset store
  AssetStore(AssetStore &) = delete;
  AssetStore(AssetStore &&) = delete;

  AssetStore& operator=(const AssetStore&) = delete;
  AssetStore& operator=(AssetStore&&) = delete;


  /* Textures */
  void loadTexture(std::unique_ptr<Renderer> &renderer, const std::string &key, const std::string &path);
  void loadTexture(std::unique_ptr<Renderer> &renderer,std::string &&key, const std::string &path);
  auto getTexture(const std::string &key) const -> SDL_Texture*;
  void removeTexture(const std::string &key);
  void clearTextures();
  auto getTextureNames() const -> std::vector<std::string>;
  auto texturesEmpty() const -> bool;

  /* Fonts */
  void loadFont(const std::string &key, const std::string &path);
  void loadFont(std::string &&key, const std::string &path);
  auto getFont(const std::string &key, const FontSize size) const -> TTF_Font*;
  void removeFont(const std::string &key);
  void clearFonts();
  auto fontsEmpty() const -> bool;

  private:
    std::unordered_map<std::string, SDL_Texture*> m_textures;
    std::unordered_map<std::string, std::array<TTF_Font*, constants::Fonts::NB_SIZES>> m_fonts;

  void cleanupTextures();
  void cleanupFonts();
};

// -> TTF_Fonts* pointers [0, 1, 2, 3, 4, 5]
// -> 

#endif
