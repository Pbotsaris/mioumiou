#include "application.hpp"
#include "projects/project_config.hpp"
#include "projects/project.hpp"

Application::Application() :
  m_engine(Project::init("./sandbox_project")) {
   m_lua.open_libraries(sol::lib::base, sol::lib::math);
}

void Application::run(){

  if(!m_engine.valid()){
    return;
  };

  m_engine.setup(m_lua);
  m_engine.run();
};

Application::~Application(){
  ProjectConfig::cleanup();
}
