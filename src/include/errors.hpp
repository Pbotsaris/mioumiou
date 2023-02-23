#ifndef ERRORS_H
#define ERRORS_H
#include <string>
#include <tuple>

class Error { // NOLINT

public:
  virtual ~Error() = default;
  [[nodiscard]] auto virtual error() const -> std::string = 0;
};

class ImageDrawError : public Error {

public:
  explicit ImageDrawError(std::string &&msg);
  [[nodiscard]] auto error() const -> std::string override;

private:
  std::string m_msg;
};

#endif
