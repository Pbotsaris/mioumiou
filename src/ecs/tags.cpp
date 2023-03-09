#include "tags.hpp"
#include <spdlog/spdlog.h>
#include <doctest.h>

/* Tag & Group Management */

/**
 *  Tags a GameObject.
 *  @param GameObject
 *  @param TagName
 *
 */

void Tags::add(GameObject gameObject, const TagName &tag) {
  m_gameObjectTag.emplace(gameObject.id(), tag);
  m_tagGameObject.emplace(tag, gameObject);
}

/**
 *  Returns whether a GameObject has a tag.
 *  @param GameObject
 *  @param TagName
 *  @return boolean
 *
 */

auto Tags::has(GameObject gameObject,
                                    const TagName &tag) const -> bool {
  // first search whether the game object has any tag
  if (m_gameObjectTag.find(gameObject.id()) == m_gameObjectTag.end()) {
    return false;
  }

  auto foundObj = m_tagGameObject.find(tag);

  if (foundObj == m_tagGameObject.end()) {
    return false;
  }

  return foundObj->second == gameObject;
}

/**
 *  Gets a game object by TagName
 *  @param TagName
 *  @return GameObject
 *
 */

auto Tags::getBy(const TagName &tag) const -> GameObject {
  return m_tagGameObject.find(tag)->second;
}

/**
 *  Disassociates a TagName from a GameObject
 *  @param GameObject
 *  @param TagName
 */

void Tags::remove(GameObject gameObject) {

  auto tag = m_gameObjectTag.find(gameObject.id());

  if (tag != m_gameObjectTag.end()) {
    m_gameObjectTag.erase(gameObject.id());
    m_tagGameObject.erase(tag->second);
    return;
  }
  spdlog::debug( "Could not remove tag from GameObject id {} because it contain no tags.", gameObject.id());
}


TEST_CASE("Tags"){ //NOLINT

    SUBCASE("Tag GameObjects") {

    auto obj = GameObject(1);
    Tags tags;

    tags.add(obj, "player");

    CHECK(tags.has(obj, "player") == true);
    CHECK(tags.has(obj, "bad_name") == false);

    auto objWithoutATag = GameObject(2);

    CHECK(tags.has(objWithoutATag, "player") == false);

    auto foundObject = tags.getBy("player");
    CHECK(foundObject == obj);
    CHECK(foundObject != objWithoutATag);

    tags.remove(obj);
    CHECK(tags.has(obj, "player") == false);

    tags.remove(objWithoutATag); // shall not crash
  }
}
