#ifndef CAMERA_H
#define CAMERA_H

#include <cstdint>

class Camera {

  public:
  struct Position {
    std::int32_t x = 0;
    std::int32_t y = 0;

    Position(std::int32_t x, std::int32_t y) : x(x), y(y){}; //NOLINT
  };

  struct Dimension {
    std::int32_t width = 0;
    std::int32_t height = 0;

    Dimension(std::int32_t width, std::int32_t height) : width(width), height(height){}; //NOLINT
  };

  explicit Camera(Position &&position, Dimension &&dimension) : m_position(position), m_dimension(dimension){}

  void setX(int32_t x); //NOLINT
  void setY(int32_t y); //NOLINT

  [[nodiscard]] auto x() const -> std::int32_t;
  [[nodiscard]] auto y() const -> std::int32_t;
  [[nodiscard]] auto width() const -> std::int32_t;
  [[nodiscard]] auto height() const -> std::int32_t;
  
  private:
    Position m_position;
    Dimension m_dimension;
};

#endif
