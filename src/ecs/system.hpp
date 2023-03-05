#ifndef SYSTEM_H
#define SYSTEM_H
#include "signature.hpp"
#include "game_object.hpp"
#include "component.hpp"
#include <vector>

class System {

public:
  System() = default;
  virtual ~System() = default;

  System(System &&) = default;
  System(const System &) = default;
  System& operator=(const System &) = default;
  System& operator=(System &&) = default;

  [[nodiscard]] virtual auto name() const -> std::string = 0;

  void addGameObject(GameObject gameObject);
  void removeGameObject(GameObject gameObject);

  [[nodiscard]] auto gameObjects() const -> std::vector<GameObject>;
  [[nodiscard]] auto componentSignature() const -> const Signature&;

  template<typename T> void requiredComponent();

private:
  Signature m_componentSignature;
  std::vector<GameObject> m_gameObjects;
};

template <typename T> void System::requiredComponent() {
  const auto componentId = Component<T>::id();
  m_componentSignature.set(componentId);
}

#endif
