#define DOCTEST_CONFIG_IMPLEMENT
#include "ecs/component.hpp"
#include "ecs/game_object.hpp"
#include "ecs/system.hpp"
#include "game/game.hpp"
#include "utils/tester.hpp"

int main(int argc, char *argv[]) {
  /* Tests with -test */
  int res = Tester::test(argc, argv);

  /* set log level -log=debug etc */
  Tester::logDebug(argc, argv);

  if (res > 0) {
    return res;
  }

  /* return early on tests */
  if (Tester::didTest(argc, argv)) {
    return 0;
  }

  Game game;
  game.run();

  return 0;
}
