#ifndef TYPE_INFO_H
#define TYPE_INFO_H

#include <string>
#include <typeinfo>
#include <cxxabi.h>

struct TypeInfo {

  template <typename T> static auto typeToString() -> std::string {
    int status = 0;
    char *realname = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
    std::string typeName(realname);
    free(realname); //NOLINT

   
    return typeName;
  }
};


#endif
