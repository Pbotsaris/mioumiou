#include "path.hpp"
#include <filesystem>

namespace fs = std::filesystem;

auto PathUtils::pathExists(const std::string &path) -> bool {
  return fs::exists(fs::path{path});
}
