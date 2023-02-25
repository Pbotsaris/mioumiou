#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <cstdint>

// forward declaring so GameObjs can hold a pointer to the manager
class WorldManager;

class GameObject {
public:

  explicit GameObject(uint32_t id); // NOLINT
  explicit GameObject(uint32_t id, WorldManager *wm); // NOLINT

  [[nodiscard]] auto id() const -> uint32_t;
  [[nodiscard]] auto wm() const -> WorldManager*;

  auto operator==(const GameObject &other) const -> bool;
  auto operator!=(const GameObject &other) const -> bool;
  auto operator>(const GameObject &other) const -> bool;
  auto operator<(const GameObject &other) const -> bool;

  /* Implemented in world_manager.hpp as leverages m_wm that is forward declared here */
  template<typename T, typename ...TARGS>void addComponent(TARGS&& ...args);
  template<typename T>void removeComponent();
  template<typename T>[[nodiscard]] auto hasComponent() const -> bool;
  template<typename T>[[nodiscard]] auto getComponent() const -> T&;


private:
  std::uint32_t m_id;
  WorldManager *m_wm = nullptr; 
};

#endif
