#ifndef UTILS_PATH_H
#define UTILS_PATH_H

#include <string>

struct PathUtils {
  static auto pathExists(const std::string &path)->bool;
  static auto pathAppend(const std::string &left, const std::string &right) -> std::string;
};


#endif
