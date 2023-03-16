#define DOCTEST_CONFIG_IMPLEMENT
#include "game/game.hpp"
#include "utils/tester.hpp"
#include <sol.hpp>
#include <exception>


int main(int argc, char *argv[]) {

  /* ./mioumiou -test */
  int res = Tester::test(argc, argv);

  if (Tester::didTest(argc, argv)) {
    return res;
  }

  Game game;
  game.run();

  return 0;
}
