#ifndef INFO_COMPONENT_H
#define INFO_COMPONENT_H

#include <string>
#include "utils/constants.hpp"

struct InfoComponent {
  std::string name;

  explicit InfoComponent(std::string name = constants::Defaults::Info::NAME ) : name(std::move(name)){}
};

#endif
