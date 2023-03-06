#ifndef GAME_H
#define GAME_H
#include "camera.hpp"
#include "ecs/world_manager.hpp"
#include "event_system/event_bus.hpp"
#include "game/asset_store.hpp"
#include "map_builder.hpp"
#include "renderer.hpp"
#include <memory>

struct MapTileCount {
  int32_t xAxis;
  int32_t yAxis;
};

struct MapDimension {
  int32_t width;
  int32_t height;
};

class Game {

public:
  Game();
  ~Game();

  Game(Game &) = delete;
  Game(Game &&) = delete;

  void operator=(Game &) = delete;
  void operator=(Game &&) = delete;

  void setup();
  void run();
  void update();
  void render();
  void processInput();
  void loadLevel(uint32_t level);

  [[nodiscard]] auto isValid() const -> bool;

  constexpr static const size_t WINDOW_WIDTH = 800;
  constexpr static const size_t WINDOW_HEIGHT = 640;

  constexpr static const MapTileCount MAP_TILE_COUNT = {.xAxis = 25,
                                                        .yAxis = 20};

  /* the size of tiles are defined by resolution and number number of tiles
   * in the map files (i.e. ./assets/tilemaps/jungle.map)
   * */
  constexpr static const TileDimension TILE_SIZE = {
      .width = WINDOW_WIDTH / MAP_TILE_COUNT.xAxis,
      .height = WINDOW_HEIGHT / MAP_TILE_COUNT.yAxis,
      .scale = 2};

  constexpr static const MapDimension MAP_DIMENSION = {
      .width = MAP_TILE_COUNT.xAxis * TILE_SIZE.width * TILE_SIZE.scale,
      .height = MAP_TILE_COUNT.yAxis * TILE_SIZE.height * TILE_SIZE.scale,
  };

private:
  bool m_isRunning = false;
  uint32_t m_prevFrameTime = 0;

  std::unique_ptr<Window> m_window;
  std::unique_ptr<Renderer> m_renderer;
  std::unique_ptr<WorldManager> m_wm;
  std::unique_ptr<AssetStore> m_store;
  std::unique_ptr<EventBus> m_eventBus;

  Camera m_camera;

  void addEventListeners();

  void capFrameRate() const;
  [[nodiscard]] auto deltatime() const -> double;

  constexpr static const int8_t FPS = 60;
  constexpr static const double MILLISECS = 1000.0;
  constexpr static const int8_t MSECS_PER_FRAME = MILLISECS / FPS; // NOLINT: narrowing to int8_t
};

#endif

/* Read about layers for the future
 *
Add a layer component to entities: You can add a layer component to each entity
to specify which layer it belongs to. The layer component can be a simple
integer that represents the layer index or a more complex structure that
includes additional information such as the layer's name, visibility, and
properties. Then, you can use this component to filter entities by layer when
performing queries and operations.

Implement a layer manager system: You can create a separate system or subsystem
that manages layers in your ECS. This system can keep track of all the layers in
your game or application and provide methods for adding, removing, and querying
entities in each layer. The layer manager system can also handle tasks such as
sorting entities by layer depth or applying layer-specific effects and
behaviors.

Use a spatial partitioning algorithm: A spatial partitioning algorithm such as
quadtree or octree can be used to divide the game world into separate layers or
regions. Each layer or region can be represented by a separate partition, and
entities can be assigned to the partition that corresponds to their position.
This approach can help improve performance by reducing the number of entities
that need to be processed or rendered at once.

Use a combination of the above approaches: Depending on the complexity of your
ECS and the specific requirements of your game or application, you may need to
use a combination of the above approaches to implement layers effectively. For
example, you can use a layer component to tag entities with their layer index,
use a layer manager system to handle layer-specific tasks, and use a spatial
partitioning algorithm to optimize entity processing and rendering.
*/
