#include "camera.hpp"
#include <spdlog/spdlog.h>



void Camera::setX(int32_t x){ //NOLINT
  m_position.x = x;
}

void Camera::setY(int32_t y){ //NOLINT
  m_position.y = y;
}


 auto Camera::x() const -> std::int32_t{
   return m_position.x;
 }

 auto Camera::y() const -> std::int32_t{
   return m_position.y;
 }


 auto Camera::width() const -> std::int32_t{
   return m_dimension.width;
 }

 auto Camera::height() const -> std::int32_t{
   return m_dimension.height;
 }


