#include "world_manager.hpp"
#include "components/rigid_body_component.hpp"
#include "components/transform_component.hpp"
#include "doctest.h"
#include "spdlog/spdlog.h"

/**
 * Creates a new GameObject in the World.
 * Newly created objects are queue in m_gameObjectAddQueue and are only
 * introcuced in the world when the WorldManager::update is called.
 *
 * @return  The created GameObject
 */

auto WorldManager::createGameObject() -> GameObject {
  uint32_t id = m_gameObjectCount++; // NOLINT: id is too short

  if (m_gameObjectCount >= m_gameObjectcomponentSignatures.size()) {
    // resize component signature vec
    m_gameObjectcomponentSignatures.resize(m_gameObjectCount + 1);
  }
  GameObject gameObject(
      id, this); // give a sequential id and a reference to the worldManager

  // we queue the game object to be added by update
  m_gameObjectAddQueue.insert(gameObject);

  spdlog::info("GameObject id '{}' created and queued for insert.",
               gameObject.id());

  return gameObject;
}

/**
 *  Updates the world: add GameObjects queued to be added
 *  and removes GameObjects queued to be removed.
 *
 */

void WorldManager::update() {

  for (const auto gameObject : m_gameObjectAddQueue) {
    gameObjectToSystems(gameObject);
  }

  m_gameObjectAddQueue.clear();
}

/**
 *  Get the current number of GameObjects in the world.
 *  @return uint32_t
 *
 */

auto WorldManager::gameObjectCount() const -> std::uint32_t {
  return m_gameObjectCount;
}

/* PRIVATE */

/**
 * Adds a GameObject System where the GameObject Signatures satifies the
 * Components requirements in System::m_componentSignature. a GameObject
 * signature must contain all system component requirements in its signature.
 * @param gameObject: A GameObject
 */

void WorldManager::gameObjectToSystems(GameObject gameObject) {
  const Signature &gameObjSig =
      m_gameObjectcomponentSignatures.at(gameObject.id());

  for (auto &keypair : m_systems) {
    std::shared_ptr<System> sys = keypair.second;

    /*
     *  NOTE:
     *  The bitwise &(AND) between the GameObject Signature and System Signature
     * must be equals to System Signature otherwise the GameObject either don't
     * have all components required by the System or has components that the
     * System does not support.
     */

    bool satisfied = (sys->componentSignature() & gameObjSig) == sys->componentSignature();

    if (satisfied) {
      sys->addGameObject(gameObject);
    }
  }
}

// unit tests only

TEST_CASE("World Manager") { // NOLINT

  WorldManager wm; // NOLINT: Short var

  SUBCASE("Create GameObject and add component") {
    auto obj = wm.createGameObject();

    CHECK(obj.id() == 0); // first obj must be 0
    CHECK(wm.gameObjectCount() == 1);
    wm.addComponent<TransformComponent>(obj, glm::vec2(1, 1), glm::vec2(3, 3),
                                        0.453); // NOLINT: constants
                                                //
    wm.addComponent<RigidBodyComponent>(obj, glm::vec2(2.0, 2.0)); // NOLINT

    CHECK(wm.hasComponent<TransformComponent>(obj) == true);
    CHECK(wm.hasComponent<RigidBodyComponent>(obj) == true);

    auto obj2 = wm.createGameObject();
    CHECK(obj2.id() == 1); // first obj must be 0
    CHECK(wm.gameObjectCount() == 2);

    auto comp = wm.getComponent<RigidBodyComponent>(obj);

    CHECK(comp.velocity == glm::vec2(2.0, 2.0)); // NOLINT
  }


  SUBCASE("Remove component from obj") {

    auto obj = wm.createGameObject();
    wm.addComponent<RigidBodyComponent>(obj, glm::vec2(2.0, 2.0)); // NOLINT
                                                                   //
    CHECK(wm.hasComponent<RigidBodyComponent>(obj) == true);
    wm.removeComponent<RigidBodyComponent>(obj);
    CHECK(wm.hasComponent<RigidBodyComponent>(obj) == false);

  }

  SUBCASE("Add component via GameObject") {
    auto obj = wm.createGameObject();
    obj.addComponent<RigidBodyComponent>(glm::vec2(3.0, 3.0)); // NOLINT

    auto c = obj.getComponent<RigidBodyComponent>(); // NOLINT
    CHECK(c.velocity == glm::vec2(3.0, 3.0));         // NOLINT
  }


  SUBCASE("Remove component via GameObject") {
    auto obj = wm.createGameObject();
    obj.addComponent<RigidBodyComponent>(glm::vec2(3.0, 3.0)); // NOLINT

    CHECK(obj.hasComponent<RigidBodyComponent>() == true);
    obj.removeComponent<RigidBodyComponent>();
    CHECK(obj.hasComponent<RigidBodyComponent>() == false);
  }

}
