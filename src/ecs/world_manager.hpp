#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <cstdint>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <vector>

#include "ecs/signature.hpp"
#include "ecs/game_object.hpp"
#include "ecs/pool.hpp"
#include "ecs/system.hpp"

class WorldManager {

  public:
    WorldManager() = default;


    /* game objects are only actually added to game world when update is called */
    void update();

    /* INFO: GameObject Management */
    auto createGameObject() -> GameObject;
    void removeGameObject(GameObject gameObject);
    /* */

    
    /* INFO: Component Management */
    template<typename T, typename ...TARGS> void addComponent(GameObject gameObject, TARGS &&...args);
    template<typename T> void RemoveComponent(GameObject gameObject);
    template<typename T> auto getComponent(GameObject gameObject) const -> T&;
    template<typename T> auto hasComponent(GameObject gameObject) const -> bool;
    /* */

    void addSystem(System sys);

  private:
    std::uint32_t m_gameObjectCount = 0;

    /* Components pooled by type 
     * [vector index = Components<T>::id()], [pool index = GameObject::id()]
     * */
    std::vector<PoolInterface*> m_componentPools;

    /* Signature will allow manager to to know what components are set for a given GameObject 
     * vector index = GameObject::id()
     * m_gameObjectcomponentSignatures[GameObject::id()] = componentIds;
     * */
    std::vector<Signature> m_gameObjectcomponentSignatures;

    /* map of system where key = System type using std::type_index */
    std::unordered_map<std::type_index, System*> systems;

    /* sets as queues insertion and removal of GameObjects */
    std::set<GameObject> m_gameObjectAddQueue;
    std::set<GameObject> m_gameObjectRemoveQueue;
};

#endif
