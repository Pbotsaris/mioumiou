#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <glm.hpp>
#include <imgui.h>
#include <iostream>
#include <sol.hpp>

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);

  sol::state lua;

  lua.open_libraries(sol::lib::base);

  glm::vec2 vel = glm::vec2(5.0 - 2.5);

  std::cout << vel.g << std::endl;
  return 0;

  SDL_Quit();
}
