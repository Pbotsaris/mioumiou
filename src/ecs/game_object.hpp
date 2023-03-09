#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <cstdint>
#include <vector>
#include "types/types.hpp"

/* forward declaring so GameObjs can hold a pointer to the manager */
class WorldManager;

class GameObject {
public:

  /* this contructor is called for tests only. m_wm will never be nullptr */
  explicit GameObject(uint32_t id); // NOLINT
                                    
  explicit GameObject(uint32_t id, WorldManager *wm); // NOLINT

  [[nodiscard]] auto id() const -> GameObjectId;
  [[nodiscard]] auto worldManager() const ->  WorldManager*;

  void remove();

  void tag(const TagName &tag);
  void removeTag();
  [[nodiscard]] auto hasTag(const TagName &tag) const -> bool;

  void toGroup(const GroupName &group);
  void ungroup(const GroupName &group);
  void ungroupAll();
  [[nodiscard]] auto belongsTo(const GroupName &group) const -> bool;
  [[nodiscard]] auto groups() const -> std::vector<GroupName>;

  void joinAlliance(const GroupName &alliance);
  void leaveAlliance(const GroupName &alliance);
  void leaveEveryAlliance();
  [[nodiscard]] auto hasAlliance(const GroupName &alliance) -> bool;
  [[nodiscard]] auto isAllied(const GameObject &other) const -> bool;
  [[nodiscard]] auto alliances() const -> std::vector<GroupName>;

  auto operator==(const GameObject &other) const -> bool;
  auto operator!=(const GameObject &other) const -> bool;
  auto operator>(const GameObject &other) const -> bool;
  auto operator<(const GameObject &other) const -> bool;

  /* Implemented in world_manager.hpp as leverages m_wm that is forward declared here */
  template<typename T, typename ...TARGS>void addComponent(TARGS&& ...args);
  template<typename T>void removeComponent();
  template<typename T>[[nodiscard]] auto hasComponent() const -> bool;
  template<typename T>[[nodiscard]] auto getComponent() const -> T&;

private:
  GameObjectId m_id;
  WorldManager *m_wm = nullptr; 
};

#endif
