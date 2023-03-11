#ifndef COLORS_H
#define COLORS_H
#include <cstdint>
#include <tuple>
#include <SDL2/SDL.h>

//struct SDL_Color {
//  uint8_t r;
//  uint8_t g;
//  uint8_t b;
//  uint8_t a;
//};
//
class Colors { // NOLINT
  public:
  virtual ~Colors() = default;

  [[nodiscard]] auto virtual getColor() const -> SDL_Color = 0;

};

class Black : public Colors {
  public:
  Black() noexcept;
  [[nodiscard]] auto getColor() const -> SDL_Color override;

  private:
  SDL_Color m_color;
};

class Gray : public Colors {
  public:
  Gray() noexcept;
  [[nodiscard]] auto getColor() const -> SDL_Color override;

  private:
  SDL_Color m_color;
};

class White : public Colors {
  public:
  White() noexcept;
  [[nodiscard]] auto getColor() const -> SDL_Color override;

  private:
  SDL_Color m_color;
};

class Red : public Colors {
  public:
  Red() noexcept;
  [[nodiscard]] auto getColor() const -> SDL_Color override;

  private:
  SDL_Color m_color;
};

class Yellow : public Colors {
  public:
  Yellow() noexcept;
  [[nodiscard]] auto getColor() const -> SDL_Color override;

  private:
  SDL_Color m_color;
};

class Green : public Colors {
  public:
  Green() noexcept;
  [[nodiscard]] auto getColor() const -> SDL_Color override;

  private:
  SDL_Color m_color;
};


#endif
