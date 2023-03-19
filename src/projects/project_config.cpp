#include "project_config.hpp"
#include <sol.hpp>
#include <spdlog/spdlog.h>
#include "utils/path.hpp"
#include "utils/constants.hpp"

using namespace constants;

ProjectConfig *ProjectConfig::m_instance = nullptr; //NOLINT

auto ProjectConfig::getInstance() -> ProjectConfig* {

  if(m_instance == nullptr){
    m_instance = new ProjectConfig(); //NOLINT
  }
  
  return m_instance;
};

void ProjectConfig::cleanup(){
  delete m_instance; // NOLINT
};


void ProjectConfig::load(const std::string &path){

 if (!PathUtils::pathExists(path)) {
    spdlog::error("'{}' is not a valid path. Could not load lua Project Config file.", path);
    return;
  }

 std::string configPath = PathUtils::pathAppend(path, Files::CONFIG_FILE);

 if (!PathUtils::pathExists(configPath)) {
    spdlog::error("Config files does not exist in '{}'. Could not load Project Config.", configPath);
    return;
  }


  sol::state lua;
  lua.open_libraries(sol::lib::base);

  sol::load_result res = lua.load_file(configPath);

 if (!res.valid()) {
    sol::error err = res;
    spdlog::error("There's been an error config lua file in '{}'. Project Config not loaded. Error:\n{}", configPath, err.what());
    return;
  }

  lua.script_file(configPath);

  sol::optional<sol::table> projectOpt = lua[Lua::PROJECT];

  if(projectOpt == sol::nullopt){
    spdlog::error("In file: '{}': Could not find '{}' table. Project Config not loaded.", configPath, Lua::PROJECT);
    return;
  }

  sol::table project = projectOpt.value();
  sol::optional<std::string> nameOpt = project[Lua::Project::NAME];
  sol::optional<std::string> pathOpt = project[Lua::Project::PATH];
  sol::optional<int32_t> fpsOpt = project[Lua::Project::FPS];
  sol::optional<sol::table> windowOpt = project[Lua::Project::WINDOW];

  if(nameOpt != sol::nullopt){
    m_name = nameOpt.value();
  } else {
    spdlog::warn("In file: '{}': Could not find '{}' value. Using default.", configPath, Lua::Project::NAME);
  };

  if(pathOpt != sol::nullopt){
     if(PathUtils::pathExists(pathOpt.value())){
      m_absolutePath = pathOpt.value();
    } else {
      spdlog::warn("In file: '{}': Path '{}' does not exist. Loading default.", configPath, pathOpt.value());
    } 

  } else {
    spdlog::warn("In file: '{}': Could not find '{}' value. Using default.", configPath, Lua::Project::PATH);
  };

  if(fpsOpt != sol::nullopt){
    m_fps = fpsOpt.value();
  } else {
    spdlog::warn("In file: '{}': Could not find '{}' value. Using default.", configPath, Lua::Project::FPS);
  };

if(windowOpt != sol::nullopt){
    sol::optional<int32_t> widthOpt = windowOpt.value()[Lua::GenericTable::Size::WIDTH];
    sol::optional<int32_t> heightOpt = windowOpt.value()[Lua::GenericTable::Size::HEIGHT];
    
    if(widthOpt != sol::nullopt){
      m_window.width =  widthOpt.value();
    } else {
      spdlog::warn("In file: '{}': Could not find '{}' value. Using default.", configPath, Lua::GenericTable::Size::WIDTH);
    }

    if(heightOpt != sol::nullopt){
      m_window.height =  heightOpt.value();
    } else {
      spdlog::warn("In file: '{}': Could not find '{}' value. Using default.", configPath, Lua::GenericTable::Size::HEIGHT);
    }

  } else {
    spdlog::warn("In file: '{}': Could not find '{}' value. Using default.", configPath, Lua::Project::WINDOW);
  };

    spdlog::info("Project '{}' configuration in '{}' was loaded successfully.", m_name, configPath);
    m_loaded = true;
};

auto ProjectConfig::fps() const -> int32_t { return m_fps; }
auto ProjectConfig::loaded() const -> bool { return m_loaded; }
auto ProjectConfig::windowWidth() const -> int32_t { return m_window.width; }
auto ProjectConfig::windowHeight() const -> int32_t { return m_window.height; }
auto ProjectConfig::window() const -> WindowResolution { return m_window; }
auto ProjectConfig::path() const -> std::string { return m_absolutePath; }
auto ProjectConfig::name() const -> std::string { return m_name; }
