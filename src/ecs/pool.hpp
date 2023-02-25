#ifndef POOL_H
#define POOL_H
#include "components/transform_component.hpp"
#include <iostream>
#include <vector>

const static std::size_t DEFAULT_POOL_SIZE = 100;

// Interface to add Pool<T> in one container in WorldManager
class PoolInterface { // NOLINT: no constructors for interfaces
public:
  virtual ~PoolInterface() = default;
};

template <typename T>

class Pool : public PoolInterface {

public:
  explicit Pool(std::size_t size = DEFAULT_POOL_SIZE) { m_data.resize(size); }

  Pool(const Pool &) = default;
  Pool(const Pool &&) = delete;

  Pool& operator=(const Pool&) = default;
  Pool& operator=(Pool&&) = delete;

  ~Pool() override = default;

  // no copying or moving

  auto empty() -> bool { return m_data; };
  auto size() -> std::size_t { return m_data.size(); };
  auto at(uint32_t index) -> T & { return m_data.at(index); }
  auto operator[](uint32_t index) -> T & { return m_data[index]; }

  void resize(std::size_t size) { m_data.resize(size); };
  void clear() { m_data.clear(); };
  void add(T value) { m_data.push_back(value); }
  void set(uint32_t index, T value) { m_data.at(index) = value; }

private:
  std::vector<T> m_data;
};

#endif
