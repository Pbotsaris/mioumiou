#include "world_manager.hpp"
#include "components/all.hpp"
#include "doctest.h"
#include "spdlog/spdlog.h"
#include "systems/all.hpp"

/**
 * Creates a new GameObject in the World.
 * Newly created objects are queue in m_gameObjectAddQueue and are only
 * introcuced in the world when the WorldManager::update is called.
 *
 * @return  The created GameObject
 */

auto WorldManager::createGameObject() -> GameObject {

  uint32_t id = makeId(); // NOLINT: short variable

  GameObject gameObject(id, this); // give objs reference to the WorldManager

  m_gameObjectAddQueue.insert(gameObject);
  spdlog::debug("GameObject id '{}' created and queued for insert.", gameObject.id());

  return gameObject;
}

/**
 *  Removes a new GameObject in the World.
 *  This method only queues GameObject for removal
 *  GameObject will be removed upon calling the WordManager::update method by
 * the game loop ids of removed GameObjects are stored in m_freedGameObjectIds
 * for later reuse
 * @param  the Game object to remove
 */

void WorldManager::removeGameObject(GameObject gameObject) {
  m_gameObjectRemoveQueue.insert(gameObject);
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

  for (const auto gameObject : m_gameObjectRemoveQueue) {
    removeGameObjectFromSystems(gameObject);
    m_gameObjectcomponentSignatures.at(gameObject.id()).reset();
    m_freedGameObjectIds.push_back(gameObject.id()); // avail for reuse

    /* remove from all groups & tags */
    m_tags.remove(gameObject);
    m_groups.removeFromAllGroups(gameObject);
    m_alliances.removeFromAllGroups(gameObject);
  }

  m_gameObjectAddQueue.clear();
  m_gameObjectRemoveQueue.clear();
}

auto WorldManager::groups() -> Groups&{
  return m_groups;
}

auto WorldManager::alliances() -> Groups&{
  return m_alliances;
}

auto WorldManager::tags() -> Tags& {
  return m_tags;
}

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

    bool satisfied =
        (sys->componentSignature() & gameObjSig) == sys->componentSignature();

    // spdlog::debug("GameObject id '{}' added to {}.", gameObject.id(),
    // sys->name());

    if (satisfied) {
      spdlog::debug("GameObject id '{}' added to {}.", gameObject.id(), sys->name());
      sys->addGameObject(gameObject);
    }
  }
}

/**
 * Removes a GameObject from all systems that is has been registered to.
 * @param gameObject: A GameObject
 */

void WorldManager::removeGameObjectFromSystems(const GameObject &gameObject) {
  const Signature &gameObjSig =
      m_gameObjectcomponentSignatures.at(gameObject.id());

  for (auto &keypair : m_systems) {
    std::shared_ptr<System> sys = keypair.second;

    bool satisfied =
        (sys->componentSignature() & gameObjSig) == sys->componentSignature();

    /* we only removed object exists in system */
    if (satisfied) {
      sys->removeGameObject(gameObject);
    }
  }
}


/**
 * Removes a GameObject from all pools
 * @param gameObject: A GameObject
 */

void WorldManager::removeGameObjectFromPools(const GameObject &gameObject){

  for(auto &pool : m_componentPools){
    pool->remove(gameObject.id()); 
  }
}

/**
 * Makes an ID from a newly created GameObject
 * Method will check m_freedGameObjectIds from available ids to be reused
 * otherwise creates a new one.
 *
 * @return uint32_t GameObject ID
 */

auto WorldManager::makeId() -> uint32_t {

  if (!m_freedGameObjectIds.empty()) {
    uint32_t id = m_freedGameObjectIds.front(); // NOLINT: short variable
    m_freedGameObjectIds.pop_front();
    return id;
  }

  uint32_t id = m_gameObjectCount++; // NOLINT: short variable

  // resize component signature vec
  if (m_gameObjectCount >= m_gameObjectcomponentSignatures.size()) {
    m_gameObjectcomponentSignatures.resize(m_gameObjectCount + 1);
  }
  return id;
}

/* UNIT TESTS */

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
    CHECK(c.velocity == glm::vec2(3.0, 3.0));        // NOLINT
  }

  SUBCASE("Remove component via GameObject") {
    auto obj = wm.createGameObject();
    obj.addComponent<RigidBodyComponent>(glm::vec2(3.0, 3.0)); // NOLINT

    CHECK(obj.hasComponent<RigidBodyComponent>() == true);
    obj.removeComponent<RigidBodyComponent>();
    CHECK(obj.hasComponent<RigidBodyComponent>() == false);
  }

  SUBCASE("create component and add its respective system") {

    auto obj = wm.createGameObject();

    obj.addComponent<SpriteComponent>();
    obj.addComponent<AnimationComponent>();
    obj.addComponent<RigidBodyComponent>();
    obj.addComponent<TransformComponent>();

    wm.createSystem<AnimationSystem>();

    // update must come before we get the system
    wm.update();

    auto sys = wm.getSystem<AnimationSystem>();
    CHECK(sys.gameObjects().size() != 0);
    CHECK(sys.gameObjects()[0] == obj);
  }
}
