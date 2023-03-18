#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <cstdint>
#include <iostream>
#include <set>
#include <deque>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>

#include "ecs/signature.hpp"
#include "ecs/game_object.hpp"
#include "ecs/pool.hpp"
#include "ecs/system.hpp"
#include "component.hpp"
#include "groups.hpp"
#include "tags.hpp"
#include "utils/typeInfo.hpp"

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

    auto groups() -> Groups&;
    auto alliances() -> Groups&;
    auto tags() -> Tags&;

  private:
    std::uint32_t m_gameObjectCount = 0;

    /* DATA STRUCTURES */

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

    /* destroyed objects ids availale for resuse */
    std::deque<uint32_t> m_freedGameObjectIds;

    /* - */

    /* GameObject may belong to multiple groups or alliances */
    Groups m_groups;
    Groups m_alliances;
    Tags m_tags;

    void gameObjectToSystems(GameObject gameObject);
    void removeGameObjectFromSystems(const GameObject &gameObject);
    void removeGameObjectFromPools(const GameObject &gameObject);

    auto makeId() -> uint32_t;
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
 */

template <typename T, typename... TARGS>
void WorldManager::addComponent(GameObject gameObject, TARGS &&...args) {

  const uint32_t componentId = Component<T>::id();

  if (componentId >= m_componentPools.size()) {
    m_componentPools.resize(componentId + 1, nullptr);
  }


  if (m_componentPools.at(componentId) == nullptr) {
    m_componentPools[componentId] = std::make_shared<ComponentPool<T>>();
  }

  auto pool = std::static_pointer_cast<ComponentPool<T>>(m_componentPools.at(componentId));

  T newComponent(std::forward<TARGS>(args)...);
  // or T newComponent = std::move(T(std::forward<TARGS>(args)...));

  pool->set(gameObject.id(), newComponent);

  // set the component signature for this gameObject with the added component
  m_gameObjectcomponentSignatures.at(gameObject.id()).set(componentId);

  spdlog::debug("Component id '{}' of type '{}' was added to GameObject id '{}'",
      componentId,
      TypeInfo::typeToString<T>(),
      gameObject.id());
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

  /* pack pool */
  auto pool = m_componentPools.at(componentId);
  pool->remove(gameObject.id()); // method is virtual no need for ptr casting

  /* Set bitset to zero */
  m_gameObjectcomponentSignatures.at(gameObject.id()).set(componentId, false);

  spdlog::debug("Component id '{}' of type '{}' was removed to GameObject id '{}'",
      componentId,
      TypeInfo::typeToString<T>(),
      gameObject.id());

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
auto WorldManager::getComponent(GameObject gameObject) const -> T& {
  const uint32_t componentId = Component<T>::id();

  // TODO:  accessing non existing 
 
  if(m_componentPools.size() <= componentId){
    spdlog::critical("The program attempted to the ComponentPool type '{}' that does not exist."
        "This is an unrecoverable error.", TypeInfo::typeToString<T>()
        );
    throw;
  }

  std::shared_ptr<ComponentPool<T>> pool =
      std::static_pointer_cast<ComponentPool<T>>(m_componentPools.at(componentId));

    return pool->at(gameObject.id());
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
  bool ok = m_systems.insert(std::make_pair(std::type_index(typeid(T)), sys)).second;//NOLINT: short var

  if (!ok){
    spdlog::warn("failed to insert system id '{}' to Systems map.");
  }
}

/**
 * Removes a System type <T> from the world.
 *
 * @tparam T: A System type
 */

template <typename T> void WorldManager::removeSystem() {
  auto key = std::type_index(typeid(T));

  if(m_systems.contains(key)){
  m_systems.erase(key);
  return;
  }

  spdlog::warn("Unable to remove a System from m_systems.");
}

/**
 * Checks wether a System<T> exists in the world or not.
 *
 * @tparam T: A System type
 * @return: boolean
 */

template <typename T> auto WorldManager::hasSystem() const -> bool {
  return m_systems.contains(std::type_index(typeid(T)));
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
   auto key = std::type_index(typeid(T));

  if(! m_systems.contains(key)){
    spdlog::critical("Failed to get System from Systems map.");
  }

  return *(std::static_pointer_cast<T>( m_systems.find(key)->second));
}

/** Same as the above but for callable directly by GameObject */

template<typename T, typename ...TARGS>
void GameObject::addComponent(TARGS&&...args){
  m_wm->addComponent<T>(*this, args...);
}

template<typename T>
void GameObject::removeComponent(){
  m_wm->removeComponent<T>(*this);
}

template<typename T>
auto GameObject::hasComponent() const -> bool{
return m_wm->hasComponent<T>(*this);
}

template<typename T>
auto GameObject::getComponent() const -> T&{
  return m_wm->getComponent<T>(*this);
}

#endif
