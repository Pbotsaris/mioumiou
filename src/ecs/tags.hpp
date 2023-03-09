#ifndef TAGS_H
#define TAGS_H

#include "game_object.hpp"
#include "types/types.hpp"
#include <unordered_map>

class Tags {
  public:
    void add(GameObject gameObject, const TagName &tag);
    [[nodiscard]] auto has(GameObject gameObject, const TagName &tag) const -> bool;
    [[nodiscard]] auto getBy(const TagName &tag) const -> GameObject;
    void remove(GameObject gameObject);

  private:
    /* GameObject may only have a single tag */
    std::unordered_map<GameObjectId, TagName> m_gameObjectTag;
    std::unordered_map<TagName, GameObject>   m_tagGameObject;

};

#endif


