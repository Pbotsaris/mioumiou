#ifndef COLLISION_EVENT_H
#define COLLISION_EVENT_H

#include "ecs/game_object.hpp"
#include "event_system/event.hpp"

class CollisionEvent : public Event {
  public:
   

    CollisionEvent(GameObject a, GameObject b) : m_a(a), m_b(b) {} //NOLINT

    [[nodiscard]] auto a() const -> GameObject { return m_a; }
    [[nodiscard]] auto b() const -> GameObject { return m_b; }

  private:
    GameObject m_a;
    GameObject m_b;
};

#endif 
