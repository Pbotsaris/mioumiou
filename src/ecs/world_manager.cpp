#include "world_manager.hpp"
#include "component.hpp"
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
  GameObject gameObject(id); // give a sequential id

  // we queue the game object to be added by update
  m_gameObjectAddQueue.insert(gameObject);

  spdlog::info("GameObject id '{}' created and queued for insert.",
               gameObject.id());

  return gameObject;
}

/*  we only add a gameobject to system when it contapins the components required
 * by that system so the component signature of an gameObject must match the
 * component requirements of a system
 */

void WorldManager::update() {}

/**
 * Creates a component<T> and add to a component pool Pool<T>.
 * The new component is then registered a GameObject in
 * m_gameObjectcomponentSignatures accordingly. This method will grow the
 * m_componentPools vectors as well contained Pool<T> as necessary.
 *
 * @tparam T: A Component type
 * @tparam TARGS: Component<T> constructor parameter types
 * @param gameObject: GameObject to add a component to
 * @param args: Component<T> constructor parameters
 * @return void
 */

template <typename T, typename... TARGS>
void WorldManager::addComponent(GameObject gameObject, TARGS &&...args) {

  const uint32_t componentId = Component<T>::id();

  if (componentId >= m_componentPools.size()) {
    m_componentPools.resize(componentId + 1, nullptr);
  }

  if (m_componentPools.at(componentId) == nullptr) {
    m_componentPools[componentId] = std::move(new Pool<T>());
  }

  Pool<T> *pool = Pool<T>(m_componentPools.at(componentId));

  // grow pool to the current total number of objects in the world
  if (gameObject.id() >= pool->size()) {
    pool->resize(m_gameObjectCount);
  }

  T newComponent(std::forward<TARGS>(args)...);
  // or T newComponent = std::move(T(std::forward<TARGS>(args)...));

  pool->set(gameObject.id(), newComponent);

  // set the component signature for this gameObject with the added component
  m_gameObjectcomponentSignatures.at(gameObject.id()).set(componentId);
}

/**
 * Removes a Component<T> from a GameObject
 * m_gameObjectcomponentSignatures bitset for the GameObject in question.
 * Note the the component pool remains unmodified.
 *
 * @tparam T: A Component type
 * @param gameObject: A GameObject to remove a Component<T> from
 * @return void
 */

template <typename T>
void WorldManager::removeComponent(GameObject gameObject) {
  const uint32_t componentId = Component<T>::id();

  // set to 0
  m_gameObjectcomponentSignatures.at(gameObject.id()).set(componentId, false);
}

/**
 * Checks if a GameObject has either Component<T> or not.
 *
 * @tparam T: A Component type
 * @param gameObject: A GameObject to check the Component<T> existance
 * @return boolean
 */

template <typename T>
auto WorldManager::hasComponent(GameObject gameObject) const -> bool {
  const uint32_t componentId = Component<T>::id();
  return m_gameObjectcomponentSignatures.at(gameObject.id()).test(componentId);
}

/**
 * Gets a Component<T> from a GameObject
 *
 * @tparam T: A Component type
 * @param gameObject:  GameObject to get a Component<T> from
 * @return Component<T> reference
 */

template <typename T>
auto WorldManager::getComponent(GameObject gameObject) const -> T & {
  const uint32_t componentId = Component<T>::id();

  Pool<T> pool = m_componentPools.at(componentId);

  if (hasComponent<T>(gameObject)) {
    return *(std::static_pointer_cast<T>(pool.at(gameObject.id())));
  }
}

/**
 * Creates a new System<T> in the world.
 *
 * @tparam T: A System type
 * @tparam TARGS: System<T> constructor argument types.
 * @param ...args: System<T> constructor arguments.
 */

template <typename T, typename... TARGS>
void WorldManager::createSystem(TARGS &&...args) {

  T sys = std::move(new T(std::forward<TARGS>(args)...));
  // TODO: check if std::type_index(*sys) would also work to simplify the syntax
  m_systems.insert(std::make_pair(std::type_index(typeid(T)), sys));
}

/**
 * Removes a System type <T> from the world.
 *
 * @tparam T: A System type
 */

template <typename T> void WorldManager::removeSystem() {
  // TODO: delete System. future smart pointer?
  m_systems.find(std::type_index(typeid(T)));
}

/**
 * Checks wether a System<T> exists in the world or not.
 *
 * @tparam T: A System type
 * @return: boolean
 */

template <typename T> auto WorldManager::hasSystem() const -> bool {
  return m_systems.find(std::type_index(typeid(T))) != m_systems.end();
}

/**
 * Gets a System<T> from the word.
 *
 * @tparam T: A System type
 * @return: A reference to the System<T>.
 */

template <typename T> auto WorldManager::getSystem() const -> T & {
  // 1. find the map iterator pointer by the key.
  // 2. get value from pair(iter pair)  with ->second
  // 3. cast to T* and dereference for return.
  return *(std::static_pointer_cast<T>(
      m_systems.find(std::type_index(typeid(T)))->second));
}

/* PRIVATE */

/**
 * Adds a GameObject System where the GameObject Signatures satifies the Components requirements
 * in System::m_componentSignature.
 * a GameObject signature must contain all system component requirements in its signature.
 * @param gameObject: A GameObject
 */


void WorldManager::gameObjectToSystems(GameObject gameObject) {
  const Signature &gameObjSig = m_gameObjectcomponentSignatures.at(gameObject.id());

  for (auto &keypair : m_systems) {
    System *sys = keypair.second;

/* 
 *  NOTE: 
 *  The bitwise &(AND) between the GameObject Signature and System Signature must be equals to System Signature 
 *  otherwise the GameObject either don't have all components required by the System
 *  or has components that the System does not support.
 */
   
    bool satisfied = (sys->componentSignature() & gameObjSig)  == sys->componentSignature();

    if (satisfied) {
      sys->addGameObject(gameObject);
    }
  }
}
