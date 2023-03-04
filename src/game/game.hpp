#ifndef GAME_H
#define GAME_H
#include <memory>
#include "game/asset_store.hpp"
#include "renderer.hpp"
#include "ecs/world_manager.hpp"
#include "map_builder.hpp"
#include "event_system/event_bus.hpp"

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

private:
  bool          m_isRunning = false; 
  uint32_t      m_prevFrameTime = 0; 

  std::unique_ptr<Window >      m_window;
  std::unique_ptr<Renderer>     m_renderer;
  std::unique_ptr<WorldManager> m_wm;
  std::unique_ptr<AssetStore>   m_store;
  std::unique_ptr<EventBus>      m_eventBus;

  void capFrameRate()const;
  [[nodiscard]] auto deltatime() const -> double;

  constexpr static const int8_t       FPS = 60; 
  constexpr static const int8_t       MSECS_PER_FRAME = 1000/FPS; 
  constexpr static const double       MILLISECS =  1000.0;

  constexpr static const size_t       WINDOW_WIDTH = 800;
  constexpr static const size_t       WINDOW_HEIGHT = 640;
  constexpr static const MapDimension NB_TILES = {.width = 25, .height = 20};

  /* the size of tiles are defined by resolution and number number of tiles
   * in the map files (i.e. ./assets/tilemaps/jungle.map)
   * */
  constexpr static const TileDimension TILE = {
                                                .width =  WINDOW_WIDTH / NB_TILES.width,
                                                .height =  WINDOW_HEIGHT / NB_TILES.height,
  };
};

#endif


/* Read about layers for the future
 *
Add a layer component to entities: You can add a layer component to each entity to specify which layer it belongs to.
The layer component can be a simple integer that represents the layer index or a more complex structure that includes additional information such as the layer's name, visibility, and properties.
Then, you can use this component to filter entities by layer when performing queries and operations.

Implement a layer manager system: You can create a separate system or subsystem that manages layers in your ECS. This system can keep track of all the layers in your
game or application and provide methods for adding, removing, and querying entities in each layer. The layer manager system can also handle tasks such as sorting entities by layer depth
or applying layer-specific effects and behaviors.

Use a spatial partitioning algorithm: A spatial partitioning algorithm such as quadtree or octree can be used to divide the game world into separate layers or regions. Each layer or
region can be represented by a separate partition, and entities can be assigned to the partition that corresponds to their position. This approach can help improve performance by reducing the
number of entities that need to be processed or rendered at once.

Use a combination of the above approaches: Depending on the complexity of your ECS and the specific requirements of your game or application, you may need to use a combination
of the above approaches to implement layers effectively. For example, you can use a layer component to tag entities with their layer index, use a layer manager system to handle
layer-specific tasks, and use a spatial partitioning algorithm to optimize entity processing and rendering.
*/
