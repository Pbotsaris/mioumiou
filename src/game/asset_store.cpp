#include <SDL2/SDL_image.h>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>

#include "asset_store.hpp"

/* Textures */

void AssetStore::loadTexture(std::unique_ptr<Renderer> &renderer, std::string &&key, const std::string &path) {

  SDL_Texture *tex = renderer->createTexture(path);

  if (tex == nullptr) {
    spdlog::error("Failed to load texture id '{}' from '{}'", key, path);
    return;
  }
  bool ok = m_textures.insert({key, tex}).second; // NOLINT: short var

  if (!ok) {
    spdlog::warn( "Failed insert texture id '{}' to AssetStore. Texture already exist", key);
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

void AssetStore::clearTextures() {
  cleanupTextures();
  m_textures.clear();
}

/* Fonts */

void AssetStore::loadFont(std::string &&key, const std::string &path) {

  /* Will include all font sizes defined in constants::Fonts::Sizes */
  std::array<TTF_Font *, constants::Fonts::NB_SIZES> fonts{};

  uint32_t index = 0;

  for (const auto fontSize : constants::Fonts::SIZES) {

    TTF_Font *font = TTF_OpenFont(path.c_str(), fontSize);

    if (font == nullptr) {
      spdlog::error("Failed to load Font '{}' from '{}'. Error: {}", key, path, TTF_GetError());

    }

    if (index < fonts.size()) {
      fonts[index] = font; // NOLINT: checking bounds so warning in meaningless here

    } else {
      spdlog::critical( "Index out of bounds of Font array. Index '{}', Array Size '{}'.", index, fonts.size());

    }

    index++;
  }

  bool ok = m_fonts.insert({key, std::move(fonts)}).second; // NOLINT: short var

  if (!ok) {
    spdlog::warn("Failed insert Font '{}' to AssetStore. Font already exist", key);

    /* clean up on error */
    for (auto *font : fonts) {
      TTF_CloseFont(font);
    }
  }
}

auto AssetStore::getFont(const std::string &key, FontSize size) const -> TTF_Font * {

  if (m_fonts.contains(key)) {
    return m_fonts.at(key)[size];
  };

  spdlog::error("Could not get Font '{}'. I does not exist in AssetStore.",
                key);
  return nullptr;
};

void AssetStore::removeFont(const std::string &key) {
  if (m_fonts.contains(key)) {

    for (auto *font : m_fonts.at(key)) {
      TTF_CloseFont(font);
    }

    m_fonts.erase(key);
    return;
  }

  spdlog::error("Could not get Font '{}'. I does not exist in AssetStore.", key);
}

void AssetStore::clearFonts() {
  cleanupFonts();
  m_fonts.clear();
}

AssetStore::~AssetStore() {
  cleanupTextures();
  cleanupFonts();
}

/* Private */

void AssetStore::cleanupTextures() {
  for (auto &texture : m_textures) {
    SDL_DestroyTexture(texture.second);
  }
}

void AssetStore::cleanupFonts() {
  for (auto &fontSizes : m_fonts) {
    for(auto *font : fontSizes.second){
       TTF_CloseFont(font);
    }
  }
}
