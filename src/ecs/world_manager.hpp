#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <cstdint>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>

#include "ecs/signature.hpp"
#include "ecs/game_object.hpp"
#include "ecs/pool.hpp"
#include "ecs/system.hpp"
#include "component.hpp"

class WorldManager {

  public:
    WorldManager() = default;

    /* game objects are only actually added to game world when update is called */
    void update();
    auto gameObjectCount() const -> std::uint32_t ;

    /* GameObject Management */
    auto createGameObject() -> GameObject;
    void removeGameObject(GameObject gameObject);
    /* */

    
    /* Component Management */
    template<typename T, typename ...TARGS> void addComponent(GameObject gameObject, TARGS &&...args);
    template<typename T> void removeComponent(GameObject gameObject);
    template<typename T> auto getComponent(GameObject gameObject) const -> T&;
    template<typename T> auto hasComponent(GameObject gameObject) const -> bool;
    /* */

    /* System Management */
    template<typename T, typename ...TARGS> void createSystem(TARGS &&...args);
    template<typename T> void removeSystem();
    template<typename T> auto hasSystem() const -> bool;
    template<typename T> auto getSystem() const -> T&;
    /*  */

  private:
    std::uint32_t m_gameObjectCount = 0;

    /* Components pooled by type 
     * [vector index = Components<T>::id()], [pool index = GameObject::id()]
     * */

    std::vector<std::shared_ptr<PoolInterface>> m_componentPools;

    /* Signature will allow manager to to know what components are set for a given GameObject 
     * vector index = GameObject::id()
     * m_gameObjectcomponentSignatures[GameObject::id()] = componentIds;
     * */

    std::vector<Signature> m_gameObjectcomponentSignatures;

    /* map of system where key = System type using std::type_index */
    std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems;

    /* sets as queues insertion and removal of GameObjects */
    std::set<GameObject> m_gameObjectAddQueue;
    std::set<GameObject> m_gameObjectRemoveQueue;


    void gameObjectToSystems(GameObject gameObject);
};

/** Templated method implementation from here **/

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
    m_componentPools[componentId] = std::make_shared<Pool<T>>();
  }

  std::shared_ptr<Pool<T>> pool =
      std::static_pointer_cast<Pool<T>>(m_componentPools.at(componentId));

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

  std::shared_ptr<Pool<T>> pool =
      std::static_pointer_cast<Pool<T>>(m_componentPools.at(componentId));

  if (hasComponent<T>(gameObject)) {
    return *(std::static_pointer_cast<T>(pool->at(gameObject.id())));
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

  std::shared_ptr<T> sys = std::make_shared<T>(std::forward<TARGS>(args)...);
  // TODO: check if std::type_index(*sys) would also work to simplify the syntax
  m_systems.insert(std::make_pair(std::type_index(typeid(T)), sys));
}

/**
 * Removes a System type <T> from the world.
 *
 * @tparam T: A System type
 */

template <typename T> void WorldManager::removeSystem() {
  m_systems.erase(std::type_index(typeid(T)));
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

#endif
