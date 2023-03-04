#include "game_object.hpp"
#include "world_manager.hpp"

GameObject::GameObject(uint32_t id) : m_id(id) {} // NOLINT: id is too short

GameObject::GameObject(uint32_t id, WorldManager *wm) : m_id(id), m_wm(wm) {} // NOLINT: id is too short

auto GameObject::id() const -> uint32_t { return m_id; }

void GameObject::remove(){
  m_wm->removeGameObject(*this);
}

/* NOTE: these operator overloads are also required 
 *       as we are inserting GameObjects in a std::set
 * */
auto GameObject::operator==(const GameObject &other) const -> bool {
  return other.id() == m_id;
}

auto GameObject::operator!=(const GameObject &other) const -> bool {
  return other.id() != m_id;
}

auto GameObject::operator>(const GameObject &other) const -> bool {
  return other.id() > m_id;
}

auto GameObject::operator<(const GameObject &other) const -> bool {
  return other.id() > m_id;
}


