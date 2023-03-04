#ifndef COLORS_H
#define COLORS_H
#include <cstdint>
#include <tuple>

struct RGBA {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

class Colors { // NOLINT
  public:
  virtual ~Colors() = default;

  [[nodiscard]] auto virtual getColor() const -> RGBA = 0;

};

class Black : public Colors {
  public:
  Black() noexcept;
  [[nodiscard]] auto getColor() const -> RGBA override;

  private:
  RGBA m_color;
};

class Gray : public Colors {
  public:
  Gray() noexcept;
  [[nodiscard]] auto getColor() const -> RGBA override;

  private:
  RGBA m_color;
};

class White : public Colors {
  public:
  White() noexcept;
  [[nodiscard]] auto getColor() const -> RGBA override;

  private:
  RGBA m_color;
};

class Red : public Colors {
  public:
  Red() noexcept;
  [[nodiscard]] auto getColor() const -> RGBA override;

  private:
  RGBA m_color;
};

class Yellow : public Colors {
  public:
  Yellow() noexcept;
  [[nodiscard]] auto getColor() const -> RGBA override;

  private:
  RGBA m_color;
};


#endif
