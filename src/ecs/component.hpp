#ifndef COMPONENT_H
#define COMPONENT_H

#include <bitset>
#include <cstdint>
#include <components/transform_component.hpp>

struct baseComponent {
protected:
  inline static uint32_t nextId = 0; // NOLINT: cannot be const
};


// template will return a unique ID per T
template <typename T> class Component : public baseComponent {
public:
  [[nodiscard]] static auto id() -> uint32_t {
    static uint32_t uniqueId = baseComponent::nextId++;
    return uniqueId;
  }
};

#endif
