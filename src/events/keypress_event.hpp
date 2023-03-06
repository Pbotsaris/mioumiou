#ifndef KEYDOWN_EVENT_H
#define KEYDOWN_EVENT_H

#include <SDL2/SDL.h>

#include "ecs/game_object.hpp"
#include "event_system/event.hpp"


class KeyDownEvent : public Event{

  public:
    explicit KeyDownEvent(SDL_Keycode key, std::uint16_t modifier) : m_key(key), m_modifier(modifier) {} //NOLINT

    [[nodiscard]] auto key() const -> SDL_Keycode { return m_key; }
    [[nodiscard]] auto keyChar() const -> char { return static_cast<char>(m_key); }
    [[nodiscard]] auto modifier() const -> std::uint16_t { return m_modifier; }

  private: 
    SDL_Keycode    m_key;
    std::uint16_t  m_modifier;

};

#endif
