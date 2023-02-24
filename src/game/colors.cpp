#include "colors.hpp"

Black::Black() noexcept : m_color({.r = 0, .g = 0, .b = 0, .a = 255}) {}  //NOLINT
Gray::Gray() noexcept : m_color({.r = 21, .g = 21, .b = 21, .a = 255}) {} //NOLINT
White::White() noexcept : m_color({.r = 255, .g = 255, .b = 255, .a = 255}) {} //NOLINT
                                                                          //
auto Black::getColor() const -> RGBA {return m_color;}
auto Gray::getColor() const -> RGBA {return m_color;}
auto White::getColor() const -> RGBA {return m_color;}
