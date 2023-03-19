#include "path.hpp"
#include <filesystem>

namespace fs = std::filesystem;

auto PathUtils::pathExists(const std::string &path) -> bool {
  return fs::exists(fs::path{path});
}

auto PathUtils::pathAppend(const std::string &left, const std::string &right) -> std::string { // NOLINT
  fs::path leftPath {left};
  fs::path rightPath {right};

  return (leftPath /= rightPath).string();
}
