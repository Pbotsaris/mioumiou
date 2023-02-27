#ifndef TESTER_H
#define TESTER_H

#include "doctest.h"
#include <iostream>
#include <string>

struct Tester {

  static auto test(int argc, char **argv) -> int {
    return doTest(argc > 1 &&
                  (std::string(argv[1]) == "--test")); // NOLINT: raw strings
  }

  static auto didTest(int argc, char **argv) -> bool {
    return argc > 1 && willTest(std::string(argv[1])); // NOLINT: raw string
  }


private:

  static auto willTest(std::string &&argv) -> bool { return argv == "--test"; }

  static auto doTest(bool t_will_test) -> int {
    if (!t_will_test) {
      return 0;
    }

    doctest::Context ctx;
    ctx.setOption("abort-after", 5); // NOLINT: Num Constant
    ctx.setOption("no-breaks", true);

    int res = ctx.run();

    if (ctx.shouldExit()) {
      return res;
    }

    return -1;
  }
};

#endif // !TESTER_H
