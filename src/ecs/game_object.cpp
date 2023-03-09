#include "game_object.hpp"
#include "world_manager.hpp"
#include <set>
#include <algorithm>

GameObject::GameObject(uint32_t id) : m_id(id) {} // NOLINT: id is too short

GameObject::GameObject(uint32_t id, WorldManager *wm) : m_id(id), m_wm(wm) {} // NOLINT: id is too short

auto GameObject::id() const -> GameObjectId { return m_id; }

void GameObject::remove(){ m_wm->removeGameObject(*this); }

void GameObject::tag(const TagName &tag){
  m_wm->tags().add(*this, tag);
}

auto GameObject::hasTag(const TagName &tag) const -> bool {
  return m_wm->tags().has(*this, tag);
}

void GameObject::removeTag(){
  m_wm->tags().remove(*this);
}

void GameObject::toGroup(const GroupName &group) {
  m_wm->groups().add(*this, group);
}

auto GameObject::groups() const -> std::vector<GroupName>{
 return m_wm->groups().getGameObjectsGroups(*this);
};

auto GameObject::belongsTo(const GroupName &group) const -> bool {
 return m_wm->groups().belongs(*this, group);
}

void GameObject::ungroup(const GroupName &group) {
  m_wm->groups().remove(*this, group);
}

void GameObject::ungroupAll(){
  m_wm->groups().removeFromAllGroups(*this);
}

void GameObject::joinAlliance(const GroupName &alliance){
  m_wm->alliances().add(*this, alliance);
}

auto GameObject::alliances() const -> std::vector<GroupName>{
 return m_wm->alliances().getGameObjectsGroups(*this);
}

auto GameObject::hasAlliance(const GroupName &alliance) -> bool {
  return m_wm->alliances().belongs(*this, alliance);
}

auto GameObject::isAllied(const GameObject &other) const -> bool{

  std::set<GroupName> intersection;

  auto thisAlliances = m_wm->alliances().getGameObjectsGroups(*this);
  auto otherAlliances = m_wm->alliances().getGameObjectsGroups(other);

  std::set_intersection(
       thisAlliances.begin(), thisAlliances.end(),
       otherAlliances.begin(), otherAlliances.end(),
       std::inserter(intersection, intersection.begin())
      );

    return !intersection.empty();
}

void GameObject::leaveAlliance(const GroupName &alliance){
  m_wm->alliances().remove(*this, alliance);
}

void GameObject::leaveEveryAlliance(){
  m_wm->alliances().removeFromAllGroups(*this);
}

auto GameObject::worldManager() const -> WorldManager* {
  return  m_wm;
}

/* NOTE: these operator overloads are also required 
 *       as we are inserting GameObjects in a std::set
 * */
auto GameObject::operator==(const GameObject &other) const -> bool {
  return other.id() == m_id;
}

auto GameObject::operator!=(const GameObject &other) const -> bool {
  return other.id() != m_id;
}

auto GameObject::operator>(const GameObject &other) const -> bool {
  return other.id() > m_id;
}

auto GameObject::operator<(const GameObject &other) const -> bool {
  return other.id() > m_id;
}


