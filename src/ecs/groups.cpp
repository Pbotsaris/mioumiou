#include"groups.hpp"
#include <spdlog/spdlog.h>
#include <doctest.h>

/**
 *  Adds a GameObject to a group GroupName.
 *  @param GameObject
 *  @param GroupName
 *
 */

void Groups::add(GameObject gameObject, const GroupName &group) {

  auto gameObjects = m_gameObjectsByGroup.find(group);
  auto groups = m_groupsByGameObjectId.find(gameObject.id());
  
  if(gameObjects == m_gameObjectsByGroup.end()){
    m_gameObjectsByGroup.emplace(group, std::set<GameObject>());
  }

  if(groups == m_groupsByGameObjectId.end()){
    m_groupsByGameObjectId.emplace(gameObject.id(), std::set<GroupName>());
  }

  m_groupsByGameObjectId[gameObject.id()].insert(group);
  m_gameObjectsByGroup[group].insert(gameObject);
}

/**
 *  Returns whether a GameObject is in a group.
 *  @param GameObject
 *  @param GroupName
 *  @return boolean
 *
 */

auto Groups::belongs(GameObject gameObject, const GroupName &group) const -> bool {

  auto gameObjects = m_gameObjectsByGroup.find(group);

  if(gameObjects == m_gameObjectsByGroup.end()){
     spdlog::debug("Group '{}' does not exist.", group);
    return false;
  }

  auto foundGameObject = gameObjects->second.find(gameObject);

  if(foundGameObject != gameObjects->second.end()){
    return gameObject == *foundGameObject;
  }

  return false;
}

/**
 *  Gets all GameObjects in a group
 *  @param GroupName
 *  @return std::vector<GameObject>
 *
 */

auto Groups::getGroupGameObjects(const GroupName &group) const -> std::vector<GameObject> {
  auto gameObjects = m_gameObjectsByGroup.find(group);

  if (gameObjects != m_gameObjectsByGroup.end()) {
    return {gameObjects->second.begin(), gameObjects->second.end()};
  }

  spdlog::debug("Could not find Group '{}'. Returning an empty vector", group);
  return {};
};


auto Groups::getGameObjectsGroups(GameObject gameObject)  const -> std::vector<GroupName>{
  auto groups = m_groupsByGameObjectId.find(gameObject.id());

  if(groups != m_groupsByGameObjectId.end()){
    return {groups->second.begin(), groups->second.end()};
  }
  spdlog::debug("Could not find GameObject id '{}'. Returning an empty vector", gameObject.id());
  return {};
}

void Groups::removeFromAllGroups(GameObject gameObject) {

  auto groups = m_groupsByGameObjectId.find(gameObject.id());

  if(groups == m_groupsByGameObjectId.end()){
    spdlog::debug("GameObject id '{}' is not part of any groups. Could not remove.", gameObject.id());
    return;
  }

  /* remove from m_gameObjectByGroup data structure */
  for (const auto &groupName : groups->second){
  
    auto foundGameObjects = m_gameObjectsByGroup.find(groupName);

    if(foundGameObjects == m_gameObjectsByGroup.end()) {
      // NOTE: This happens only data structures are out of sync. Issue a critical error.
      spdlog::critical("GameObject id '{}' not in Group '{}'. Failed to remove.", gameObject.id(), groupName);
      continue;
    };
   
    m_gameObjectsByGroup[groupName].erase(gameObject);
  }

  m_groupsByGameObjectId.erase(gameObject.id());
}

void Groups::remove(GameObject gameObject, const GroupName &group) {
  
  auto foundGameObjects = m_gameObjectsByGroup.find(group);
  auto foundGroup = m_groupsByGameObjectId.find(gameObject.id());

  if (foundGameObjects == m_gameObjectsByGroup.end() || 
      foundGroup == m_groupsByGameObjectId.end()) {
    spdlog::warn("Could not remove GameObject id {} from Group '{}'. ", gameObject.id(), group);
    return;
  }

  foundGameObjects->second.erase(gameObject);
  foundGroup->second.erase(group);
}


TEST_CASE("Groups"){ //NOLINT

  Groups gps;

  SUBCASE("Add GameObject to groups") {
    auto obj1 = GameObject(1);
    auto obj2 = GameObject(2);
    auto obj3 = GameObject(3);
    auto objNotInGroup = GameObject(4);

    gps.add(obj1, "enemies");
    gps.add(obj1, "player");
    gps.add(obj2, "enemies");
    gps.add(obj3, "enemies");
    gps.add(obj3, "textures");
    gps.add(obj3, "images");

    CHECK(gps.belongs(obj1, "enemies") == true);
    CHECK(gps.belongs(obj1, "player") == true);
    CHECK(gps.belongs(obj2, "enemies") == true);
    CHECK(gps.belongs(obj3, "enemies") == true);
    CHECK(gps.belongs(obj3, "images") == true);
    CHECK(gps.belongs(obj3, "textures") == true);
    CHECK(gps.belongs(obj2, "bad_group") == false);
    CHECK(gps.belongs(objNotInGroup, "enemies") == false);

    auto objsInGroup = gps.getGroupGameObjects("enemies");

    CHECK(objsInGroup.size() == 3);
    CHECK(objsInGroup[0] == obj1);
    CHECK(objsInGroup[1] == obj2);
    CHECK(objsInGroup[2] == obj3);

    auto groups = gps.getGameObjectsGroups(obj1);
    CHECK(groups.size() == 2);
    CHECK(groups[0] == "enemies");
    CHECK(groups[1] == "player");

    gps.removeFromAllGroups(obj1);
    CHECK(gps.belongs(obj1, "enemies") == false);
    CHECK(gps.belongs(obj1, "player") == false);
    CHECK(gps.belongs(obj2, "enemies") == true);
    CHECK(gps.belongs(obj3, "enemies") == true);
    CHECK(gps.belongs(obj3, "images") == true);
    CHECK(gps.belongs(obj3, "textures") == true);

    gps.removeFromAllGroups(obj2);
    CHECK(gps.belongs(obj2, "enemies") == false);

    gps.removeFromAllGroups(obj3);
    CHECK(gps.belongs(obj3, "enemies") == false);
    CHECK(gps.belongs(obj3, "images") == false);
    CHECK(gps.belongs(obj3, "textures") == false);

    gps.add(obj3, "images");
    gps.add(obj1, "images");
    CHECK(gps.belongs(obj3, "images") == true);
    CHECK(gps.belongs(obj1, "images") == true);

    gps.removeFromAllGroups(objNotInGroup); // shall not crash
    gps.remove(obj1, "bad_group_name"); // shall not crash

    gps.add(obj1, "news");
    CHECK(gps.belongs(obj1, "news") == true);
    gps.remove(objNotInGroup, "news"); // shall not crash
    gps.remove(obj1, "news");
    CHECK(gps.belongs(obj1, "news") == false);

  }
}

