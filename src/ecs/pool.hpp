#ifndef POOL_H
#define POOL_H
#include <iostream>
#include <vector>

// Interface to add Pool<T> in one container in WorldManager
class PoolInterface { // NOLINT: no constructors for interfaces
public:
  virtual ~PoolInterface() = default;
};

template <typename T>

class Pool : public PoolInterface {

public:
  explicit Pool(std::size_t size);
   ~Pool() override;

  // no copying or moving
  Pool(Pool &) = delete;
  Pool(Pool &&) = delete;
  void operator=(Pool &) = delete;
  void operator=(Pool &&) = delete;

  auto empty() -> bool                     { return m_data; };
  auto size() -> std::size_t               { return m_data.size(); };
  auto at(uint32_t index) -> T &           { return m_data.at(index); }
  auto operator[](uint32_t index) -> T &   { return m_data[index]; }

  void resize(std::size_t size)            { m_data.resize(size); };
  void clear()                             { m_data.clear(); };
  void add(T value)                        { m_data.push_back(value); }
  void set(uint32_t index, T value)        { m_data.at(index) = value; }

private:
  std::vector<T> m_data;
};

#endif
