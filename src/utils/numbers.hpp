#ifndef NUMBERS_H
#define NUMBERS_H
#include <iostream>
#include <tuple>

class Numbers {
  public:

  template <typename T>
  static auto fromString(const std::string &num) -> std::tuple<T, bool>;

  template <typename T>
  static auto fromChar(const char character) -> std::tuple<T, bool>;

};


template <typename T>
inline auto Numbers::fromChar(const char character) -> std::tuple<T, bool>{

  std::string cstr(&character);
  return Numbers::fromString<T>(cstr);
}

template <typename T>
inline auto Numbers::fromString(const std::string &num) -> std::tuple<T, bool> {

  try {
    if (std::is_same<T, float>::value) {
      return {std::stof(num), true};

    } else if (std::is_same<T, float>::value) {
      return {std::stod(num), true};

    } else {
      return {std::stoi(num), true};
    }
  } catch (...) {
    return {-1, false};
  };
}

#endif
