#ifndef NUMBERS_H
#define NUMBERS_H

class Numbers {

  template<typename T>
  static auto fromString() -> T;
};


template<>
inline auto Numbers::fromString<int>() -> int {


}

#endif
