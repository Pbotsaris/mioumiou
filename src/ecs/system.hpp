#ifndef SYSTEM_H
#define SYSTEM_H
#include "signature.hpp"
#include "game_object.hpp"
#include <vector>

class System {

public:
  System() = default;
  virtual ~System() = default;

  // no moving or copying
  System(System &) = delete;
  System(System &&) = delete;
  void operator=(System &&) = delete;
  void operator=(System &) = delete;

  void addGameObject(GameObject gameObject);
  void removeGameObject(GameObject gameObject);

  [[nodiscard]] auto getGameObjects() const -> std::vector<GameObject>;
  [[nodiscard]] auto getComponentSignature() const -> const Signature&;

  template<typename T> void requiredComponent();

private:
  Signature m_componentSignature;
  std::vector<GameObject> m_gameObjects;
};

#endif
