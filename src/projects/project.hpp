#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include "project_config.hpp"

class Project {
  public:
   static auto init(const std::string &path) -> ProjectConfig*;
};

#endif
