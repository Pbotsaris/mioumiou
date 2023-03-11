#include "colors.hpp"

Black::Black() noexcept : m_color({.r = 0, .g = 0, .b = 0, .a = 255}) {}  //NOLINT
Gray::Gray() noexcept : m_color({.r = 21, .g = 21, .b = 21, .a = 255}) {} //NOLINT
White::White() noexcept : m_color({.r = 255, .g = 255, .b = 255}) {} //NOLINT
Red::Red() noexcept : m_color({.r = 220, .g = 20, .b = 60, .a = 255}) {} //NOLINT
Yellow::Yellow() noexcept : m_color({.r = 225, .g = 225, .b = 102, .a = 255}) {} //NOLINT
Green::Green() noexcept : m_color({.r = 34, .g = 139, .b = 34, .a = 255}) {} //NOLINT
                                                                          
auto Black::getColor() const -> SDL_Color {return m_color;}
auto Gray::getColor() const -> SDL_Color  {return m_color;}
auto White::getColor() const -> SDL_Color {return m_color;}
auto Red::getColor() const ->SDL_Color { return m_color;}
auto Yellow::getColor() const ->SDL_Color { return m_color;}
auto Green::getColor() const ->SDL_Color { return m_color;}
