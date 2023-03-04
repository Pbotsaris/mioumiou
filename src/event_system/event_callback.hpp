#ifndef EVENT_CALLBACK_H
#define EVENT_CALLBACK_H
#include "event_system/event.hpp"
#include <functional>

/* Interface to make the templated EventCallback class generic
 * and be stored in an in-common data structure
 * */

class EventCallbackInterface { // NOLINT: no constructor for interfaces

public:
  virtual ~EventCallbackInterface() = default;
  void execute(Event &event) { call(event); };

private:
  virtual void call(Event &event) = 0;
};

/*
 *  EventCallback function is wrapper around the function callback pointer
 *  The constructor takes in the function callback and the instance of the owner
 *  of the function callback (a system, for example)
 *
 *  Because EventCallback templated it also implmenets EventCallbackInterface
 *  to become polymorphic and stored in EventHandlerList.
 *
 * */

template <typename TLISTENER, typename TEVENT>
class EventCallback : public EventCallbackInterface { // NOLINT

public:
  using CallbackFunc = void (TLISTENER::*)(TEVENT &);

  EventCallback(TLISTENER *listener, CallbackFunc callback)
      : m_listener(listener), m_callback(callback) {}
  ~EventCallbackInterface() override = default;

private:
  TLISTENER *m_listener;
  CallbackFunc m_callback;

  void call(Event &event) override {
    std::invoke(m_callback, m_listener, static_cast<TEVENT &>(event));
  }
};

#endif
