#include "component_types.hpp"
#include "doctest.h"

Position::Position(glm::int32 x, glm::int32 y) : m_value(x, y){}; //NOLINT
                                                                    
auto Position::x() const -> glm::float64{ return m_value.x; } //NOLINT
auto Position::y() const -> glm::float64{ return m_value.y; } //NOLINT
auto Position::value() const -> glm::vec2{ return m_value; }
auto Position::length() const -> glm::size_t{ return m_value.length(); }


auto Position::operator >(const Position &other) const  { 
  return m_value.x > other.x() && m_value.y > other.y(); // NOLINT
} 

auto Position::operator >=(const Position &other) const  { 
  return m_value.x >= other.x() && m_value.y >= other.y(); // NOLINT
} 

auto Position::operator <(const Position &other) const  { 
  return m_value.x < other.x() && m_value.y < other.y(); // NOLINT
} 

auto Position::operator <=(const Position &other) const  { 
  return m_value.x <= other.x() && m_value.y <= other.y(); // NOLINT
} 

auto Position::operator ==(const Position &other) const  { 
  return m_value.x == other.x() && m_value.y == other.y(); // NOLINT
} 

