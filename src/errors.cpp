#include "errors.hpp"

ImageDrawError::ImageDrawError(std::string &&msg) : m_msg(msg) {}
auto ImageDrawError::error() const -> std::string { return m_msg; }
