#include <sol.hpp>
#include <exception>
#include <spdlog/spdlog.h>


int cube(int n){
  return n *n * n;
}

void testLua(){
  sol::state lua; 
  lua.open_libraries(sol::lib::base);

  lua["cube"] = &cube; // passing a c++ func to lua

  try {
  lua.script_file("./assets/scripts/config.lua");
  } catch (std::exception &error){
    spdlog::error("error: {}", error.what());
  }
   sol::table config = lua["config"];
   
   bool fullscreen = config["fullscreen"];
   int  w = config["resolution"]["width"];
   int h = config["resolution"]["height"];

   spdlog::info("fullscren: {}, resolution -> w: {}, h: {}", fullscreen, w, h);

   //  using optional
   
   sol::optional<int> val = config["value"];

   if(val != sol::nullopt){
     spdlog::info("value {}", val.value());
   }

   // using get_or to check the value
   int val2 = config["noexist"].get_or(0);
   spdlog::info("value {}", val2); // will be 0
                                   

   // lua func results
   sol::optional<sol::function> func = lua["test_func"];

   if(func != sol::nullopt){
     int res = func.value()(10);
     spdlog::info("result from lua func call = {}", res);
   }
}

