#include "system.hpp"
#include "component.hpp"
#include <spdlog/spdlog.h>
#include "doctest.h"

void System::addGameObject(GameObject gameObject) {
  m_gameObjects.push_back(gameObject);
}

void System::removeGameObject(GameObject gameObject) {

  int i = 0; // NOLINT: short var name
  int pos = -1;

  for (auto const &gobj : m_gameObjects) {

    if (gobj.id() == gameObject.id()) {
      pos = i;
    }

    i++;
  }

  if (pos > 0) {
    m_gameObjects.erase(m_gameObjects.begin() + pos);
    return;
  }

  spdlog::warn("Could not remove GameObject::id '{}' from System::m_gameObjects. it Does not exist.",
               gameObject.id());
}

auto System::getGameObjects() const -> std::vector<GameObject> {
  return m_gameObjects;
}

auto System::getComponentSignature() const -> const Signature & {
  return m_componentSignature;
}

template <typename T> void System::requiredComponent() {
  const auto componentId = Component<T>::getId();
  m_componentSignature.set(componentId);
}


TEST_CASE("System"){


  SUBCASE("Add object to game"){



  }

}

