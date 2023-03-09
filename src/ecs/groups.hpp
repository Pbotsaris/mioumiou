#ifndef GROUPS_H
#define GROUPS_H

#include "game_object.hpp"
#include "types/types.hpp"
#include <set>
#include <unordered_map>
#include <vector>

class Groups {

public:
  void add(GameObject gameObject, const GroupName &group);
  [[nodiscard]] auto belongs(GameObject gameObject, const GroupName &group) const -> bool;
  [[nodiscard]] auto getGroupGameObjects(const GroupName &group) const -> std::vector<GameObject>;
  [[nodiscard]] auto getGameObjectsGroups(GameObject gameObject) const -> std::vector<GroupName>;
  void removeFromAllGroups(GameObject gameObject);
  void remove(GameObject gameObject, const GroupName &group);

private:
  std::unordered_map<GroupName, std::set<GameObject>> m_gameObjectsByGroup;
  std::unordered_map<GameObjectId, std::set<GroupName>> m_groupsByGameObjectId;
};

#endif
