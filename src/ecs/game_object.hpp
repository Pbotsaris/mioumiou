#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <cstdint>

class GameObject {
public:
  explicit GameObject(uint32_t id); // NOLINT

  [[nodiscard]] auto id() const -> uint32_t;

  auto operator==(const GameObject &other) const -> bool;
  auto operator!=(const GameObject &other) const -> bool;
  auto operator>(const GameObject &other) const -> bool;
  auto operator<(const GameObject &other) const -> bool;

private:
  std::uint32_t m_id;
};

#endif
