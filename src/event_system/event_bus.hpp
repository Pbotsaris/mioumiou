#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <typeindex>
#include <memory>
#include <list>
#include <unordered_map>
#include "event_system/event_callback.hpp"

using EventHandlerList = std::list<EventCallbackInterface>;
using SubscriberMap = std::unordered_map<std::type_index, std::unique_ptr<EventHandlerList> >;

class EventBus {
  public:
    EventBus() = default;
    ~EventBus() = default;

    EventBus(const EventBus&) = delete;
    EventBus(const EventBus&&) = delete;
    EventBus& operator=(const EventBus&) = delete;
    EventBus& operator=(EventBus&&) = delete;

    template<typename TOWNER, typename TEVENT>
      void listenEvent(TOWNER *owner, std::function<void((TOWNER::*)(TEVENT&))> callback);

    template<typename TEVENT, typename... TARGS> 
      void dispatchEvent(TARGS&&...args);

  private:
    SubscriberMap m_listerners;
};


template<typename TOWNER, typename TEVENT> 
void EventBus::listenEvent(TOWNER *owner, std::function<void((TOWNER::*)(TEVENT&))> callback){

  if (m_listerners[typeid(TEVENT)].get() == nullptr){
    m_listerners[typeid(TEVENT)]  = std::make_unique<EventHandlerList>();
  }

  /* push subscribers to the list */
  auto subscriber = std::make_unique<EventCallback<TOWNER, TEVENT>>(owner, callback);

  /* move the ownership of the unique pointer to the list */
  m_listerners[typeid(TEVENT)]->push_back(std::move(subscriber));
}


template<typename TEVENT, typename... TARGS> 
void EventBus::dispatchEvent(TARGS&&... args){
  EventHandlerList *handlers = m_listerners[typeid(TEVENT)].get();

  if(handlers != nullptr){
    for(auto &handler : *handlers ){
      TEVENT event(std::forward<TARGS>(args)...);
      handler.execute(event);
    }
  }
}


#endif
