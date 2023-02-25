#include "game_object.hpp"

GameObject::GameObject(uint32_t id): m_id(id){} //NOLINT: id is too short

auto GameObject::id()const -> uint32_t { return m_id; }


auto GameObject::operator ==(const GameObject &other) const -> bool{
  return other.id() == m_id;
}


