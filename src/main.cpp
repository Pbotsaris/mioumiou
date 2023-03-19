#define DOCTEST_CONFIG_IMPLEMENT
#include "application.hpp"
#include "utils/tester.hpp"
#include <sol.hpp>
#include <exception>


int main(int argc, char *argv[]) {

  /* ./mioumiou -test */
  int res = Tester::test(argc, argv);

  if (Tester::didTest(argc, argv)) {
    return res;
  }

  Application app;
  app.run();

  return 0;
}
