#ifndef EVENT_CALLBACK_H
#define EVENT_CALLBACK_H
#include "event_system/event.hpp"
#include <functional>

class EventCallbackWrapperInterface { // NOLINT: no constructor for interfaces

public:
  virtual ~EventCallbackWrapperInterface() = default;
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
class EventCallbackWrapper : public EventCallbackWrapperInterface { // NOLINT

public:
  using CallbackFunc = void (TLISTENER::*)(TEVENT &);

  EventCallbackWrapper(TLISTENER *listener, CallbackFunc callback)
      : m_listener(listener), m_callback(callback) {}
  ~EventCallbackWrapper() override = default;

private:
  TLISTENER *m_listener;
  CallbackFunc m_callback;

  void call(Event &event) override {
    std::invoke(m_callback, m_listener, static_cast<TEVENT &>(event));
  }
};

#endif
