#include "game_object.hpp"

GameObject::GameObject(uint32_t id): m_id(id){} //NOLINT

auto GameObject::id()const -> uint32_t { return m_id; }
