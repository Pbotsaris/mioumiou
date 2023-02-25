#include "component.hpp"
#include "doctest.h"


TEST_CASE("Components"){ // NOLINT: Lib emits errors
  SUBCASE("Components create unique id depending on template type"){

    uint32_t id1 = Component<int>::id();
    uint32_t id2 = Component<const int>::id();
    uint32_t id3 = Component<char>::id();
    uint32_t id4 = Component<int>::id();

    // types should have different ids
    CHECK(id1 != id2); 
    CHECK(id2 != id3);
    CHECK(id1 != id3);
    CHECK(id2 != id4);
    CHECK(id3 != id4);
    // same type, same id
    CHECK(id1 == id4);

  }


}






