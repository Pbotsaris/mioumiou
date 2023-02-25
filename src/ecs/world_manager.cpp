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
 * Creates a component type T and add to a component pool Pool<T>.
 * The new component is then registered a GameObject in
 * m_gameObjectcomponentSignatures accordingly. This method will grow the
 * m_componentPools vectors as well contained Pool<T> as necessary.
 *
 * @tparam T: A Component type
 * @tparam TARGS: The Component constructor variadic arguments types
 * @param gameObject: The game object
 * @param args: the arguments to create a new component type T
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
 * Removes a component type T from a GameObject
 * m_gameObjectcomponentSignatures bitset for the GameObject in question.
 * Note the the component pool remains unmodified.
 *
 * @tparam T: A Component type
 * @param gameObject: The game object
 * @return void
 */

template <typename T>
void WorldManager::RemoveComponent(GameObject gameObject) {
  const uint32_t componentId = Component<T>::id();

  // set to 0
  m_gameObjectcomponentSignatures.at(gameObject.id()).set(componentId, false);
}

/**
 * Checks if a GameObject has either Component type <T> or not.
 *
 * @tparam T: A Component type
 * @param gameObject: The game object
 * @return boolean
 */

template <typename T>
auto WorldManager::hasComponent(GameObject gameObject) const -> bool {
  const uint32_t componentId = Component<T>::id();
  return m_gameObjectcomponentSignatures.at(gameObject.id()).test(componentId);
}

template <typename T>
auto WorldManager::getComponent(GameObject gameObject) const -> T & {
  const uint32_t componentId = Component<T>::id();

  Pool<T> pool = m_componentPools.at(componentId);

  if (hasComponent<T>(gameObject)) {
    return pool.at(gameObject.id());
  }
}
