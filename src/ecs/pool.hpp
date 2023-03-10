#ifndef POOL_H
#define POOL_H
#include <iostream>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <vector>

#include "components/transform_component.hpp"
#include "utils/constants.hpp"
#include "utils/typeInfo.hpp"

// Interface to add Pool<T> in one container in WorldManager
class PoolInterface { // NOLINT: no constructors for interfaces
public:
  virtual ~PoolInterface() = default;
  virtual void remove(uint32_t gameObjectId) = 0;
};

template <typename T>

class ComponentPool : public PoolInterface {

public:
  explicit ComponentPool(
      std::size_t size = constants::Pool::COMPONENT_POOL_SIZE)
      : m_size(0) {
    m_data.resize(size);
  }

  ComponentPool(const ComponentPool &) = default;
  ComponentPool(const ComponentPool &&) = delete;

  ComponentPool &operator=(const ComponentPool &) = default;
  ComponentPool &operator=(ComponentPool &&) = delete;

  ~ComponentPool() override = default;

  // no copying or moving

  auto empty() -> bool { return m_data.empty(); };
  auto size() -> std::size_t { return m_data.size(); };
  auto operator[](uint32_t gameObjectId) -> T & { return at(gameObjectId); }

  void clear() { 
    m_data.clear();
    m_size = 0;
  };

  void resize(std::size_t size) { 
    m_size = size;
    m_data.resize(m_size); 
  };

  void remove(uint32_t gameObjectId) override {

    auto toRemoveIndex = m_indexByGameObjectId.find(gameObjectId);

    /* remove will be called multiple times on the same obj so need this check */
    if (toRemoveIndex == m_indexByGameObjectId.end()) {
      spdlog::debug( "The GameObject id '{}' does not have any Component in Pool type {}.", gameObjectId, TypeInfo::typeToString<T>());
      return;
    }

    // swap last element with object to be removed
    auto safekeep = m_data[toRemoveIndex->second];
    auto lastElementIndex = m_size - 1;

    m_data[toRemoveIndex->second] = m_data[lastElementIndex];

    /* updating the translating maps from here */

    auto lastObjectId = m_gameObjectIdByIndex.find(lastElementIndex);

    if (lastObjectId == m_gameObjectIdByIndex.end()) {
      spdlog::critical( "The index '{}' does not exist in m_gameObjectIdByIndex map.'", m_size - 1);
      m_data[toRemoveIndex->second] = safekeep;
      return;
    }
    auto lastObjectIndex = m_indexByGameObjectId.find(lastObjectId->second);

    if (lastObjectIndex == m_indexByGameObjectId.end()) {
      spdlog::critical("The last GameObject id '{}' in m_indexByGameObjectId " "map does not exist.'", lastObjectId->second);
      m_data[toRemoveIndex->second] = safekeep;
      return;
    }

    // we are still using the index of the remove which is not the index of the last element
    m_indexByGameObjectId[lastObjectId->second] = toRemoveIndex->second;
    m_gameObjectIdByIndex[toRemoveIndex->second] = lastObjectId->second;

    m_indexByGameObjectId.erase(gameObjectId);
    m_gameObjectIdByIndex.erase(lastElementIndex);

    m_size--;
  }

  auto at(uint32_t gameObjectId) -> T & {
    auto foundIndex = m_indexByGameObjectId.find(gameObjectId);

    if (foundIndex == m_indexByGameObjectId.end()) {
      spdlog::critical("GameObject id '{}' does not have a matching index in " "Pool. Returning empty Component.",
                       gameObjectId);

      return m_dummy;
    }

    return static_cast<T&>(m_data[foundIndex->second]);
  }

  void set(uint32_t gameObjectId, T component) {

    // update
    if(m_indexByGameObjectId.find(gameObjectId) != m_indexByGameObjectId.end()){
      uint32_t index = m_indexByGameObjectId[gameObjectId];
      m_data[index] = component;
      return;
    }

    // add
    m_gameObjectIdByIndex.emplace(m_size, gameObjectId);
    m_indexByGameObjectId.emplace(gameObjectId, m_size);

    // resize
    if(m_size >= m_data.capacity()){
      m_data.resize(m_size *2);
    }

    m_data[m_size] = component;
    m_size++;
  }

private:
  std::vector<T> m_data;
  std::size_t m_size;
  T m_dummy;

  std::unordered_map<uint32_t, uint32_t> m_gameObjectIdByIndex;
  std::unordered_map<uint32_t, uint32_t> m_indexByGameObjectId;
};

#endif
