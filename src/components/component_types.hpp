#ifndef COMPONENT_TYPES_H
#define COMPONENT_TYPES_H

#include<glm.hpp>

class Position {
  glm::vec2 m_value;

  public:

  Position(glm::int32 x, glm::int32 y); //NOLINT

  [[nodiscard]] auto x() const -> glm::float64;
  [[nodiscard]] auto y() const -> glm::float64;
  [[nodiscard]] auto value() const -> glm::vec2;
  [[nodiscard]] auto length() const -> glm::size_t;

  auto operator >(const Position &other) const;
  auto operator >=(const Position &other) const;
  auto operator <(const Position &other) const;
  auto operator <=(const Position &other) const;
  auto operator ==(const Position &other) const;
};


#endif
