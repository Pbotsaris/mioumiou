#include "project.hpp"

auto Project::init(const std::string &path) -> ProjectConfig*{
  ProjectConfig *config = ProjectConfig::getInstance();
  config->load(path);
  return config;
};


