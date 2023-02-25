#include "system.hpp"
#include "doctest.h"
#include <iostream>
#include <spdlog/spdlog.h>

void System::addGameObject(GameObject gameObject) {
  m_gameObjects.push_back(gameObject);
}

void System::removeGameObject(GameObject gameObject) {

  int i = 0; // NOLINT: short var name

  for (auto const &gobj : m_gameObjects) {
    if (gobj == gameObject) { // == overload
      m_gameObjects.erase(m_gameObjects.begin() + i);
      return;
    }
    i++;
  }

  spdlog::warn("Could not remove GameObject::id '{}' from "
               "System::m_gameObjects. it Does not exist.",
               gameObject.id());
}

auto System::gameObjects() const -> std::vector<GameObject> {
  return m_gameObjects;
}

auto System::componentSignature() const -> const Signature & {
  return m_componentSignature;
}

TEST_CASE("System") { // NOLINT: ignore warnings from external libs

  System system;
  uint32_t id1 = 1;
  uint32_t id2 = 2;
  uint32_t id3 = 3;

  auto obj1 = GameObject(id1);
  auto obj2 = GameObject(id2);
  auto obj3 = GameObject(id3);

  system.addGameObject(obj1);
  system.addGameObject(obj2);
  system.addGameObject(obj3);

  SUBCASE("Add GameObject to system") {

    auto objs = system.gameObjects();
    CHECK(objs.size() == 3);
    CHECK(objs.at(0).id() == id1);
    CHECK(objs.at(1).id() == id2);
    CHECK(objs.at(2).id() == id3);
  }

  SUBCASE("Remove GameObject from system") {
    system.removeGameObject(obj1);
    auto objs = system.gameObjects();

    CHECK(objs.size() == 2);
    CHECK(objs.at(0).id() == id2);
    CHECK(objs.at(1).id() == id3);
  }
}
