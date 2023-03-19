#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

/* ProjectConfig singleton is loaded on project launch from config.lua file.
 *
 * */

#include <cstdint>
#include "utils/constants.hpp"

class ProjectConfig {

  public:
    struct WindowResolution {
      int32_t width = constants::Defaults::Resolution::WINDOW_WIDTH;
      int32_t height = constants::Defaults::Resolution::WINDOW_HEIGHT;
    };

  private:
    static ProjectConfig *m_instance; // NOLINT

    WindowResolution m_window;
    int32_t          m_fps = constants::Defaults::Resolution::FPS;
    std::string      m_name = constants::Defaults::Project::NAME;
    std::string      m_absolutePath;
    bool             m_loaded = false;
    

    ProjectConfig() = default;


  public:
    static auto getInstance() -> ProjectConfig*;
    static void cleanup();

    void load(const std::string &path);

   [[nodiscard]] auto fps() const -> int32_t;
   [[nodiscard]] auto windowWidth()  const -> int32_t;
   [[nodiscard]] auto windowHeight() const -> int32_t;
   [[nodiscard]] auto window() const -> WindowResolution;
   [[nodiscard]] auto path() const -> std::string;
   [[nodiscard]] auto name() const -> std::string;
   [[nodiscard]] auto loaded() const -> bool;

};

#endif
