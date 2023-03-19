#include "level_manager.hpp"
#include "components/all.hpp"
#include "spdlog/spdlog.h"
#include "utils/configurables.hpp"
#include "utils/constants.hpp"
#include "map_builder.hpp"
#include "level.hpp"
#include "types/containers.hpp"

using namespace constants;
using namespace configurables;

LevelManager::LevelManager(std::string path, int32_t levelNumber)
  : m_luaLevel(sol::nullopt), m_levelNumber(levelNumber), m_path(std::move(path)){

  if (!PathUtils::pathExists(m_path)) {
    spdlog::error("'{}' is not a valid path to a lua file", path);
    m_path = "";
    return;
  }

  sol::load_result res = m_lua.load_file(m_path);

  if (!res.valid()) {
    sol::error err = res;
    spdlog::error("There's been an error loading lua file '{}'. Error:\n{}", m_path, err.what());
    return;
  }

  m_lua.script_file(m_path);
  sol::optional<sol::table> levelOpt = m_lua[Lua::LEVEL];

  if (levelOpt == sol::nullopt) {
    spdlog::error( "Could not load '{}' file. Missing '{}' table. ", m_path, Lua::LEVEL);
    return;
  }

  Level *level = Level::getInstance();
  m_luaLevel   = levelOpt;

  level->setLevel(levelNumber);
};

LevelManager::~LevelManager() {}

/* */

void LevelManager::loadAssets(std::unique_ptr<AssetStore> &store, std::unique_ptr<Renderer> &renderer) {

  if (m_luaLevel == sol::nullopt) {
    spdlog::error("Cannot load level '{}' Assets. Invalid Lua state.", m_levelNumber);
    return;
  }

  sol::optional<sol::table> assets = m_luaLevel.value()[Lua::Level::ASSETS];

  if (assets == sol::nullopt) {
    spdlog::warn(" In file '{}': '{}' table must have a '{}' key.)", m_path, Lua::LEVEL, Lua::Level::ASSETS);
    return;
  }

  for(const auto &keyPair :  assets.value()){

    if(!keyPair.first.is<int>() || !keyPair.second.is<sol::table>()){
      spdlog::warn("In file '{}': Each Asset should be Lua table within another 0 indexed table.");
      break;
    }

    sol::table asset = keyPair.second.as<sol::table>();

    sol::optional<std::string> name = asset[Lua::Level::Assets::NAME];
    sol::optional<std::string> file = asset[Lua::Level::Assets::FILE];
    sol::optional<std::string> type = asset[Lua::Level::Assets::TYPE];

    if (name == sol::nullopt || file == sol::nullopt || type == sol::nullopt) {
      spdlog::warn("In file '{}': Asset table must specify a '{}', '{}' and '{}. Skipping assets.'",
                   m_path, Lua::Level::Assets::NAME, Lua::Level::Assets::FILE, Lua::Level::Assets::TYPE);
      continue;
    }

    if (!PathUtils::pathExists(file.value())) {
      spdlog::error( "In file '{}'. Path '{}' does not exist. Could not load asset.",
          m_path, file.value());

      continue;
    }

    /* Happy Path */

    if (type == Lua::Level::Assets::Type::TEXTURE) {
      store->loadTexture(renderer, name.value(), file.value());
      spdlog::info("Texture '{}' successfully loaded to the AssetStore", name.value());
    }

    if (type == Lua::Level::Assets::Type::FONT) {
      store->loadFont(name.value(), file.value());

      spdlog::info("Font '{}' successfully loaded to the AssetStore", name.value());
    }
  }
}

/* */

void LevelManager::loadMap(std::unique_ptr<WorldManager> &wm){ // NOLINT
                                                               
  if (m_luaLevel == sol::nullopt) {
    spdlog::error("Cannot load level '{} Map'. Invalid Lua state.", m_levelNumber);
    return;
  }
                                                               
  sol::optional<sol::table> mapOpt = m_luaLevel.value()[Lua::Level::MAP];

  if(mapOpt == sol::nullopt){
    spdlog::error("In file '{}': The table '{}' does not exist. Could not load map.", m_path, Lua::Level::MAP);
    return;
  }
   sol::optional<std::string>mapPath = mapOpt.value()[Lua::Level::Map::PATH];
   sol::optional<std::string>texOpt = mapOpt.value()[Lua::Level::Map::TEXTURE_NAME];
   sol::optional<int32_t>numRowsOpt = mapOpt.value()[Lua::Level::Map::NB_ROWS];
   sol::optional<int32_t>numColsOpt = mapOpt.value()[Lua::Level::Map::NB_COLS];
   sol::optional<int32_t>tileWidthOpt = mapOpt.value()[Lua::Level::Map::TILE_WIDTH];
   sol::optional<int32_t>tileHeightOpt = mapOpt.value()[Lua::Level::Map::TILE_HEIGHT];
   sol::optional<float>scaleOpt = mapOpt.value()[Lua::Level::Map::SCALE];
   sol::optional<sol::table> paddingOpt = mapOpt.value()[Lua::Level::Map::PADDING];


   if(mapPath == sol::nullopt){
      spdlog::error("In File '{}': Must provide a path to a map file in table '{}'. Failed to load map.", m_path, Lua::Level::MAP);
      return;
   }

   std::string textureName = texOpt == sol::nullopt ? Defaults::Map::TILE_TEXTURE_NAME : texOpt.value();
   warn(texOpt, Lua::Level::Map::TEXTURE_NAME, Defaults::Map::TILE_TEXTURE_NAME);

   int32_t numRows = numRowsOpt == sol::nullopt ? Defaults::Map::NB_ROWS : numRowsOpt.value();
   warn(numRowsOpt, Lua::Level::Map::NB_ROWS, Defaults::Map::NB_ROWS);

   int32_t numCols = numColsOpt == sol::nullopt ? Defaults::Map::NB_COLS : numColsOpt.value();
   warn(numColsOpt, Lua::Level::Map::NB_COLS, Defaults::Map::NB_COLS);

   int32_t tileWidth = tileWidthOpt == sol::nullopt ? Defaults::Map::Tile::WIDTH : tileWidthOpt.value();
   warn(tileWidthOpt, Lua::Level::Map::TILE_WIDTH, Defaults::Map::Tile::WIDTH);

   int32_t tileHeight = tileHeightOpt == sol::nullopt ? Defaults::Map::Tile::HEIGHT : tileHeightOpt.value();
   warn(tileHeightOpt, Lua::Level::Map::TILE_HEIGHT, Defaults::Map::Tile::HEIGHT);

   float scale = scaleOpt == sol::nullopt ? Defaults::Map::Tile::SCALE : scaleOpt.value();
   warn(scaleOpt, Lua::Level::Map::SCALE, Defaults::Map::Tile::SCALE);
   
   MapBuilder builder(mapPath.value(), textureName, {
      .width = tileWidth,
      .height = tileHeight,
      .scale = scale,
       });

   builder.build(wm);

   Level *level = Level::getInstance();

   level->setMap({
       .width =  numCols * static_cast<int32_t>(static_cast<float>(tileWidth) * scale),
       .height = numRows * static_cast<int32_t>(static_cast<float>(tileHeight) * scale),
       .padding = doPadding(paddingOpt),
       });
}

void LevelManager::loadGameObjects(std::unique_ptr<WorldManager> &wm, sol::state &lua){ // NOLINT
  
 if (m_luaLevel == sol::nullopt) {
    spdlog::error("Cannot load level '{}' GameObjects. Invalid Lua state.", m_levelNumber);
    return;
  }
                                                               
  sol::optional<sol::table> objectsOpt = m_luaLevel.value()[Lua::Level::GAME_OBJECTS];

  if(objectsOpt == sol::nullopt){
    spdlog::error("In file '{}': The table '{}' does not exist. Could not load GameObjects.", m_path, Lua::Level::GAME_OBJECTS);
    return;
  }

  for(const auto &keyPair :  objectsOpt.value()){

    if(!keyPair.first.is<int>() || !keyPair.second.is<sol::table>()){
      spdlog::error("In file '{}': Each GameObject entry must contain Lua table. Skipping...");
      break;
    }

    sol::table gameObjectTable                = keyPair.second.as<sol::table>();
    sol::optional<std::string> nameOpt        = gameObjectTable[Lua::Level::GameObject::NAME];
    sol::optional<std::string> tagOpt         = gameObjectTable[Lua::Level::GameObject::TAG];
    sol::optional<sol::table> groupOpt        = gameObjectTable[Lua::Level::GameObject::GROUPS];
    sol::optional<sol::table> alliancesOpt    = gameObjectTable[Lua::Level::GameObject::ALLIANCES];
    sol::optional<sol::table> componentsOpt   = gameObjectTable[Lua::Level::GameObject::COMPONENTS];
    GameObject gameObject                     = wm->createGameObject(nameOpt.value_or(Defaults::Info::NAME));

    if(tagOpt != sol::nullopt){
      gameObject.tag(tagOpt.value());
      spdlog::info("GameObject id '{}' was given the tag '{}'.", gameObject.id(), tagOpt.value());
    }

    if(groupOpt != sol::nullopt){
      doGroups(groupOpt.value(), gameObject);
    };

    if(alliancesOpt != sol::nullopt){
      doAlliances(alliancesOpt.value(), gameObject);
    }

    if(componentsOpt != sol::nullopt){
      doComponents(componentsOpt.value(), gameObject);

      /* Script component must be done here as we need lua state to read script files */
      const char* scriptField             = Lua::Level::GameObject::Components::SCRIPT;
      sol::optional<sol::table> scriptOpt = componentsOpt.value()[scriptField];

      /* using the GameEngine lua state here to bind callbacks */
      if(scriptOpt != sol::nullopt){
         doScript(lua, scriptOpt.value(), gameObject);
      }
    }
  } // loop
};

/* Private */

void LevelManager::doScript(sol::state &lua, sol::table &scriptTable, GameObject gameObject) const{

  const char *pathField = Lua::Level::GameObject::Components::Script::PATH;
  sol::optional<std::string> pathOpt = scriptTable[pathField];

  if(pathOpt != std::nullopt){
     if(PathUtils::pathExists(pathOpt.value())) {
        gameObject.addComponent<ScriptComponent>();
        bindScriptFunc(pathOpt.value(), lua, gameObject);

      } else {
        spdlog::warn("In File: '{}': The path to script '{}' does not exist. Failed to bind callbacks for GameObject id '{}' ",
            m_path, pathOpt.value(), gameObject.id());
      }
  } else{
     spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, pathField, gameObject.id());
     sol::optional<sol::table> dummy = lua["Level"];
  }
}

void LevelManager::bindScriptFunc(const std::string &path, sol::state &lua, GameObject gameObject){

  auto &script = gameObject.getComponent<ScriptComponent>();
  const auto info = gameObject.getComponent<InfoComponent>();
  
  sol::load_result res = lua.load_file(path);
  
    if (!res.valid()) {
      sol::error err = res;
      spdlog::error("There's been an error loading script lua file '{}'. Error:\n{}", path, err.what());
      return;
    }

  lua.script_file(path);

  /* table will have the name of the scriptable object. */
  sol::optional<sol::table> objectTableOpt = lua[info.name]; 

  if(objectTableOpt == sol::nullopt){
    spdlog::warn("Behaviour callback must be within a table with the GameObject name '{}'. Failed to bind callbacks.", info.name);
    return;
  }

  sol::optional<sol::function> onAttachOpt   = objectTableOpt.value()[Lua::Callbacks::ON_CREATE];
  sol::optional<sol::function> onUpdateOpt   = objectTableOpt.value()[Lua::Callbacks::ON_UPDATE];
  sol::optional<sol::function> onDestroyOpt  = objectTableOpt.value()[Lua::Callbacks::ON_DESTROY];
  
  if(onAttachOpt != sol::nullopt){
    script.onCreate = onAttachOpt.value();
  } else {
    spdlog::warn("Could not bind onAttach function.");
  }

   if(onUpdateOpt != sol::nullopt){
    script.onUpdate = onUpdateOpt.value();
  } else {
    spdlog::warn("Could not bind onUpdate function.");
  }

  if(onDestroyOpt != sol::nullopt){
    script.onDestroy = onDestroyOpt.value();
  } else {
    spdlog::warn("Could not bind onDestroy function.");
  }

    spdlog::info("Binded callbacks for GameObject name '{}' successfully.", info.name);
}

/* */

void LevelManager::doGroups(sol::table &groupTable, GameObject gameObject) const {

  for(const auto& keyPair : groupTable){
    if(!keyPair.first.is<int>() || !keyPair.second.is<std::string>()){
      spdlog::warn("In File: '{}': GameObject's groups must be strings within a indexed table. Groups have not been loaded.", m_path);
      break;
    }
     std::string group = keyPair.second.as<std::string>();
     spdlog::info("GameObject id '{}' was associated to group '{}'", gameObject.id(), group);
     gameObject.toGroup(group);
  };
}

/* */

void LevelManager::doAlliances(sol::table &allianceTable, GameObject gameObject) const {

  for(const auto& keyPair : allianceTable){
    if(!keyPair.first.is<int>() || !keyPair.second.is<std::string>()){
      spdlog::warn("In File: '{}': GameObject's alliances must be strings within a indexed table. Alliances have not been loaded.", m_path);
      break;
    }
     std::string alliance = keyPair.second.as<std::string>();
     spdlog::info("GameObject id '{}' has joined the '{}' alliance", gameObject.id(), alliance);
     gameObject.joinAlliance(alliance);
  };
}

/* */

void LevelManager::doComponents(sol::table &componentsTable, GameObject gameObject) const {

  sol::optional<sol::table> transformOpt   = componentsTable[Lua::Level::GameObject::Components::TRANSFORM];
  sol::optional<sol::table> rigidBodyOpt   = componentsTable[Lua::Level::GameObject::Components::RIGID_BODY];
  sol::optional<sol::table> spriteOpt      = componentsTable[Lua::Level::GameObject::Components::SPRITE];
  sol::optional<sol::table> animationOpt   = componentsTable[Lua::Level::GameObject::Components::ANIMATION];
  sol::optional<sol::table> boxcolliderOpt = componentsTable[Lua::Level::GameObject::Components::BOX_COLLIDER];
  sol::optional<sol::table> healthOpt      = componentsTable[Lua::Level::GameObject::Components::HEALTH];
  sol::optional<sol::table> healthBarOpt   = componentsTable[Lua::Level::GameObject::Components::HEALTH_BAR];
  sol::optional<sol::table> projEmitterOpt = componentsTable[Lua::Level::GameObject::Components::PROJECTILE_EMITTER];
  sol::optional<sol::table> keyControlOpt  = componentsTable[Lua::Level::GameObject::Components::KEYBOARD_CONTROL];
  sol::optional<sol::table> camFollowOpt   = componentsTable[Lua::Level::GameObject::Components::CAMERA_FOLLOW];
  //sol::optional<sol::table> scriptOpt      = componentsTable[Lua::Level::GameObject::Components::SCRIPT];

  if(transformOpt != sol::nullopt){
    doTransform(transformOpt.value(), gameObject);
    spdlog::info("Transform component attached to GameObject id '{}'", gameObject.id());
  } 

  if(rigidBodyOpt != sol::nullopt){
    doRigdBody(rigidBodyOpt.value(), gameObject);
    spdlog::info("Rigid Body component attached to GameObject id '{}'", gameObject.id());
  }

  if(spriteOpt != sol::nullopt){
    doSprite(spriteOpt.value(), gameObject);
    spdlog::info("Sprite component attached to GameObject id '{}'", gameObject.id());
  }

  if(animationOpt != sol::nullopt){
    doAnimation(animationOpt.value(), gameObject);
    spdlog::info("Animation component attached to GameObject id '{}'", gameObject.id());
  }

  if(boxcolliderOpt != sol::nullopt){
    doBoxCollider(boxcolliderOpt.value(), gameObject);
    spdlog::info("Box Collider component attached to GameObject id '{}'", gameObject.id());
  }

  if(healthOpt != sol::nullopt){
    doHealth(healthOpt.value(), gameObject);
    spdlog::info("Health component attached to GameObject id '{}'", gameObject.id());
   }

  if(healthBarOpt != sol::nullopt){
    doHealthBar(healthBarOpt.value(), gameObject);
    spdlog::info("Health Bar component attached to GameObject id '{}'", gameObject.id());
   }

  if(projEmitterOpt != sol::nullopt){
    doProjectileEmitter(projEmitterOpt.value(), gameObject);
    spdlog::info("Projectile Emitter component attached to GameObject id '{}'", gameObject.id());
  }

 if (keyControlOpt != sol::nullopt){
    doKeyboardControl(keyControlOpt.value(), gameObject);
    spdlog::info("Keyboard Control component attached to GameObject id '{}'", gameObject.id());
  }

 if (camFollowOpt != sol::nullopt){
    doCameraFollow(camFollowOpt.value(), gameObject);
    spdlog::info("Camera Follow component attached to GameObject id '{}'", gameObject.id());
  }
}

/* */

void LevelManager::doTransform(sol::table &transformTable, GameObject gameObject) const{

  const char * positionField = Lua::Level::GameObject::Components::Transform::POSITION;
  const char * scaleField = Lua::Level::GameObject::Components::Transform::SCALE;
  const char * rotationField = Lua::Level::GameObject::Components::Transform::ROTATION;

  sol::optional<sol::table> positionOpt = transformTable[positionField];
  sol::optional<sol::table> scaleOpt    = transformTable[scaleField];
  sol::optional<double> rotationOpt     = transformTable[rotationField];
  glm::vec2 position                    = Defaults::Transform::POSITION;
  glm::vec2 scale                       = Defaults::Transform::SCALE;
  double rotation                       = Defaults::Transform::ROTATION;

  if(positionOpt != sol::nullopt){
    position = doXY<int32_t>(positionOpt.value(), position, positionField);
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.", m_path, positionField, gameObject.id());
  }

 if(scaleOpt != sol::nullopt){
    scale = doXY<float>(scaleOpt.value(), scale ,scaleField);
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, scaleField, gameObject.id());
  }

 if(rotationOpt != sol::nullopt){
   rotation = rotationOpt.value();
 } else {
   spdlog::warn("In File: '{}': '{}' not present. Loading default for GameObject id '{}'.",  m_path, rotationField, gameObject.id());

 }

  gameObject.addComponent<TransformComponent>(position, scale, rotation);
}

/* */

void LevelManager::doRigdBody(sol::table &rigidBodyTable, GameObject gameObject) const{

  const char *velocityField = Lua::Level::GameObject::Components::RigidBody::VELOCITY;

  sol::optional<sol::table> velocityOpt = rigidBodyTable[velocityField];
  glm::vec2 velocity = Defaults::RigidBody::VELOCITY;

  if(velocityOpt != sol::nullopt){
    velocity = doXY<float>(velocityOpt.value(), velocity, velocityField);
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, velocityField, gameObject.id());
  }

  gameObject.addComponent<RigidBodyComponent>(velocity);
}

/* */

void LevelManager::doSprite(sol::table &spriteTable, GameObject gameObject) const{

  const char* assetNameField  = Lua::Level::GameObject::Components::Sprite::ASSET_NAME;
  const char* sizeField  = Lua::Level::GameObject::Components::Sprite::SIZE;
  const char* zPosfield  = Lua::Level::GameObject::Components::Sprite::Z_POS;
  const char* sheetField  = Lua::Level::GameObject::Components::Sprite::SHEET;
  const char* flippableField  = Lua::Level::GameObject::Components::Sprite::FLIPABLE;
  const char* fixedField  = Lua::Level::GameObject::Components::Sprite::FIXED;
  const char* cropField  = Lua::Level::GameObject::Components::Sprite::CROP;


  sol::optional<std::string> assetNameOpt = spriteTable[assetNameField];
  sol::optional<sol::table> sizeOpt       = spriteTable[sizeField];
  sol::optional<int32_t> zPosOpt          = spriteTable[zPosfield];
  sol::optional<bool> isSheetOpt          = spriteTable[sheetField];
  sol::optional<bool> flippableOpt        = spriteTable[flippableField];
  sol::optional<bool> isFixedOpt          = spriteTable[fixedField];
  sol::optional<sol::table> cropOpt       = spriteTable[cropField];

  if(assetNameOpt == sol::nullopt){
    spdlog::error("A Sprite component MUST have an '{}'. Failed to load Sprite", assetNameField);
    return;
  }

  glm::vec2 size = Defaults::Sprite::SIZE;
  int32_t zPos   = Defaults::Sprite::Z_POSITION;
  bool isSheet   = Defaults::Sprite::IS_SHEET;
  bool flippable = Defaults::Sprite::FLIPABLE;
  bool isFixed   = Defaults::Sprite::IS_FIXED;
  SDL_Rect crop  = Defaults::Sprite::CROP;

  if(sizeOpt != sol::nullopt){
   size = doWidthHeight(sizeOpt.value(), size, Lua::Level::GameObject::Components::Sprite::SIZE);
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObjct id '{}'.",  m_path, sizeField, gameObject.id());
  }

  if(zPosOpt != sol::nullopt){
    zPos = zPosOpt.value();
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, zPosfield, gameObject.id());
  }

  if(isSheetOpt != sol::nullopt){
    isSheet = isSheetOpt.value();
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, sheetField, gameObject.id());
  }

  if(flippableOpt != sol::nullopt){
    flippable = flippableOpt.value();
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, flippableField, gameObject.id());
  }


  if(isFixedOpt != sol::nullopt){
    isFixed = isFixedOpt.value();
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, fixedField, gameObject.id());
  }

  if(cropOpt != sol::nullopt){
   crop =  doCrop(cropOpt.value(), crop, cropField) ;
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, cropField, gameObject.id());
  }

  gameObject.addComponent<SpriteComponent>(assetNameOpt.value(), size, zPos, isSheet, flippable, isFixed, crop);
}

/* */

void LevelManager::doAnimation(sol::table &animationTable, GameObject gameObject) const{

   const char * framesField = Lua::Level::GameObject::Components::Animation::FRAMES;
   const char * rateField = Lua::Level::GameObject::Components::Animation::RATE;
   const char * loopableField = Lua::Level::GameObject::Components::Animation::LOOPABLE;

   sol::optional<int32_t> nbFramesOpt  = animationTable[framesField];
   sol::optional<int32_t> speedRateOpt  = animationTable[rateField];
   sol::optional<bool> loopableOpt  = animationTable[loopableField];

   int32_t nbFrames = Defaults::Animation::NB_FRAMES;
   int32_t speedRate = Defaults::Animation::FRAME_RATE_SPEED;
   bool    loopable = Defaults::Animation::LOOPABLE;


   if(nbFramesOpt != sol::nullopt){
     nbFrames = nbFramesOpt.value();
   } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, framesField, gameObject.id());
   }

  if(speedRateOpt != sol::nullopt){
     speedRate = speedRateOpt.value();
   } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, rateField, gameObject.id());
   }

  if(loopableOpt != sol::nullopt){
     loopable = loopableOpt.value();
   } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, loopableField, gameObject.id());
   }

  gameObject.addComponent<AnimationComponent>(nbFrames, speedRate, loopable);
}

//
void LevelManager::doBoxCollider(sol::table &boxColliderTable, GameObject gameObject) const{

   const char * sizeField = Lua::Level::GameObject::Components::BoxCollider::SIZE;
   const char * offsetField = Lua::Level::GameObject::Components::BoxCollider::OFFSET;

   sol::optional<sol::table> sizeOpt    = boxColliderTable[sizeField];
   sol::optional<sol::table> offsetOpt  = boxColliderTable[offsetField];

   glm::vec2 size = Defaults::BoxCollider::SIZE;
   glm::vec2 offset = Defaults::BoxCollider::OFFSET;


  if(sizeOpt != sol::nullopt){
    size = doWidthHeight(sizeOpt.value(), size, sizeField);
  } else{
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, sizeField, gameObject.id());
  }

 if(offsetOpt != sol::nullopt){
    offset = doXY<int32_t>(offsetOpt.value(), offset, offsetField);
  } else{
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, offsetField, gameObject.id());
  }

 gameObject.addComponent<BoxColliderComponent>(size, offset);
}

/* */

void LevelManager::doHealth(sol::table &healthTable, GameObject gameObject) const{
   const char * currentField = Lua::Level::GameObject::Components::Health::CURRENT;
   const char * maxField = Lua::Level::GameObject::Components::Health::MAX;

   sol::optional<int32_t> currentOpt    = healthTable[currentField];
   sol::optional<int32_t> maxOpt    = healthTable[maxField];

   int32_t current = Defaults::Health::CURRENT_HEALTH;
   int32_t max = Defaults::Health::MAX_HEALTH;

   if(currentOpt != sol::nullopt){
     current = currentOpt.value();
   } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, currentField, gameObject.id());
   }

  if(maxOpt != sol::nullopt){
     max = maxOpt.value();
   } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, maxField, gameObject.id());
   }

  gameObject.addComponent<HealthComponent>(current, max);
}

/* */

void LevelManager::doHealthBar(sol::table &healthBarTable, GameObject gameObject) const{

  const char *visibleField = Lua::Level::GameObject::Components::HealthBar::VISIBLE;
  const char *sizeField = Lua::Level::GameObject::Components::HealthBar::SIZE;
  const char *offsetField = Lua::Level::GameObject::Components::HealthBar::OFFSET;
  const char *alignField = Lua::Level::GameObject::Components::HealthBar::ALIGMENT;
  const char *textField = Lua::Level::GameObject::Components::HealthBar::TEXT;

  sol::optional<bool> visibleOpt = healthBarTable[visibleField];
  sol::optional<sol::table> sizeOpt = healthBarTable[sizeField];
  sol::optional<sol::table> offsetOpt = healthBarTable[offsetField];
  sol::optional<sol::table> textOpt = healthBarTable[textField];
  sol::optional<std::string> alignOpt = healthBarTable[alignField];

  bool visible = Defaults::HealthBar::VISIBLE;
  glm::vec2 size = Defaults::HealthBar::SIZE;
  glm::vec2 offset = Defaults::HealthBar::OFFSET;
  auto align = static_cast<HealthBarComponent::BarAlign>(Defaults::HealthBar::ALIGNMENT);


  if(visibleOpt != sol::nullopt){
    visible = visibleOpt.value();
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, visibleField, gameObject.id());
  };

  if(sizeOpt != sol::nullopt){
    size = doWidthHeight(sizeOpt.value(), size, sizeField);
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, sizeField, gameObject.id());
  };

 if(offsetOpt != sol::nullopt){
    offset = doXY<int32_t>(offsetOpt.value(), offset, offsetField);
  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, offsetField, gameObject.id());
  };

 if(alignOpt != sol::nullopt){

    auto alignOptions = Lua::Level::GameObject::Components::HealthBar::ALIGN_OPS;
    size_t pos = std::distance(alignOptions.begin(), std::find(alignOptions.begin(), alignOptions.end(), alignOpt.value()));

    if(pos < alignOptions.size()){
      align = static_cast<HealthBarComponent::BarAlign>(pos);
    } else {
     spdlog::warn("In File: '{}': Invalid '{}' options. Must be {}. Loading defaults.", m_path, alignField,
         Lua::Level::GameObject::Components::HealthBar::ALIGN_OPS_MSG);
    }

  } else {
     spdlog::warn("In File: '{}': '{}' table not present. Loading default for GameObject id '{}'.",  m_path, offsetField, gameObject.id());
  };

   gameObject.addComponent<HealthBarComponent>(visible, size, offset, align);

   if(textOpt != sol::nullopt){
     doHealthBarText(textOpt.value(), gameObject);
   }

}

/* */

void LevelManager::doHealthBarText(sol::table &healthBarTextTable, GameObject gameObject) const{

  const char *visibleField = Lua::Level::GameObject::Components::HealthBar::Text::VISIBLE;
  const char *fontField = Lua::Level::GameObject::Components::HealthBar::Text::FONT;
  const char *marginField = Lua::Level::GameObject::Components::HealthBar::Text::MARGIN;
  const char *fontSizeField = Lua::Level::GameObject::Components::HealthBar::Text::FONT_SIZE;

  sol::optional<bool> visibleOpt = healthBarTextTable[visibleField];
  sol::optional<std::string> fontOpt = healthBarTextTable[fontField];
  sol::optional<int32_t> marginOpt = healthBarTextTable[marginField];
  sol::optional<std::string> fontSizeOpt = healthBarTextTable[fontSizeField];

  bool visible = Defaults::HealthBar::TEXT_VISIBLE;
  std::string font = Defaults::HealthBar::TEXT_FONT;
  int32_t margin = Defaults::HealthBar::TEXT_MARGIN;
  Fonts::Size fontSize = Defaults::HealthBar::TEXT_FONT_SIZE;

  if(visibleOpt != sol::nullopt){
    visible = visibleOpt.value();
  } else {
     spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'." , m_path,  visibleField, gameObject.id());
  };

  if(fontOpt != sol::nullopt){
    font = fontOpt.value();
  } else {
     spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, fontField, gameObject.id());
  };

  if(marginOpt != sol::nullopt){
    margin = marginOpt.value();
  } else {
     spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, marginField, gameObject.id());
  };

  if(fontSizeOpt != sol::nullopt){

    auto fontSizeOptions = Lua::Level::GameObject::Components::HealthBar::Text::FONT_SIZE_OPS;
    size_t pos = std::distance(fontSizeOptions.begin(),
        std::find(fontSizeOptions.begin(), fontSizeOptions.end(), fontSizeOpt.value()));

    if(pos < fontSizeOptions.size()){
      fontSize = static_cast<Fonts::Size>(pos);
    } else {
     spdlog::warn("In File: '{}': Invalid '{}' options. Must be {}. Loading defaults.", m_path, fontSizeField,
         Lua::Level::GameObject::Components::HealthBar::Text::FONT_SIZE_MSG);
    }

  } else {
     spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, fontSizeField, gameObject.id());
  }

  gameObject.addComponent<HealthBarTextComponent>(visible, font, fontSize, margin);
};

/* */


void LevelManager::doProjectileEmitter(sol::table &projEmitterTable, GameObject gameObject) const{

  const char* spriteField        = Lua::Level::GameObject::Components::ProjectileEmitter::SPRITE;
  const char* sizeField          = Lua::Level::GameObject::Components::ProjectileEmitter::SIZE;
  const char* velocityField      = Lua::Level::GameObject::Components::ProjectileEmitter::VELOCITY;
  const char* freqField          = Lua::Level::GameObject::Components::ProjectileEmitter::FREQUENCY;
  const char* longField          = Lua::Level::GameObject::Components::ProjectileEmitter::LONGEVITY;
  const char* dmgField           = Lua::Level::GameObject::Components::ProjectileEmitter::PERCENT_DAMAGE;
  const char* offsetField        = Lua::Level::GameObject::Components::ProjectileEmitter::ORIGIN_OFFSET;
  const char* controllableField  = Lua::Level::GameObject::Components::ProjectileEmitter::CONTROL;


  sol::optional<std::string> spriteOpt      = projEmitterTable[spriteField];
  sol::optional<sol::table> sizeOpt         = projEmitterTable[sizeField];
  sol::optional<sol::table> velocityOpt     = projEmitterTable[velocityField];
  sol::optional<uint32_t> freqOpt           = projEmitterTable[freqField]; 
  sol::optional<uint32_t>longOpt            = projEmitterTable[longField];
  sol::optional<int32_t> dmgOpt             = projEmitterTable[dmgField];
  sol::optional<sol::table> offsetOpt       = projEmitterTable[offsetField];
  sol::optional<sol::table> controllableOpt = projEmitterTable[controllableField];

  std::string sprite;
  glm::vec2 size     = Defaults::ProjectileEmitter::SIZE;
  glm::vec2 velocity = Defaults::ProjectileEmitter::VELOCITY;
  uint32_t freq      = Defaults::ProjectileEmitter::FREQUENCY;
  uint32_t longevity = Defaults::ProjectileEmitter::LONGEVITY;
  int32_t damage     = Defaults::ProjectileEmitter::PERCENT_DAMAGE;
  glm::vec2 offset   = Defaults::ProjectileEmitter::OFFSET;


 if(spriteOpt   != sol::nullopt){
    sprite = spriteOpt.value();
   } else {
      spdlog::error("In File: '{}': The Projectile Emitter must have a '{}' field. Failed to attach component to GameObject id '{}'.",
          m_path, spriteField, gameObject.id());
      return;
   }
   if(sizeOpt != sol::nullopt) {
      size = doWidthHeight(sizeOpt.value(), size, sizeField);
   } else {
      spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, sizeField, gameObject.id());
   }

  if(velocityOpt != sol::nullopt) {
      velocity = doXY<float>(velocityOpt.value(), velocity, velocityField);
   } else {
      spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, velocityField, gameObject.id());
   }

  if(freqOpt != sol::nullopt) {
    freq = freqOpt.value();
  } else {
    spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, freqField, gameObject.id());
  }

  if(longOpt != sol::nullopt) {
    longevity = longOpt.value();
  } else {
    spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, longField, gameObject.id());
  }

  if(dmgOpt != sol::nullopt) {
    damage = dmgOpt.value();
  } else {
    spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, dmgField, gameObject.id());
  }

  if(offsetOpt != sol::nullopt) {
      offset = doXY<int32_t>(offsetOpt.value(), offset, offsetField);
   } else {
      spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, offsetField, gameObject.id());
   }

  if(controllableOpt != sol::nullopt && controllableOpt.value()) {
    doProjectileEmitterControl(controllableOpt.value(), gameObject);
  }

  gameObject.addComponent<ProjectileEmitterComponent>(sprite, size, velocity, freq, longevity, damage, offset);
}


void LevelManager::doProjectileEmitterControl(sol::table &ProjEmitterControlTable, GameObject gameObject) const{
  const char* controllableField  = Lua::Level::GameObject::Components::ProjectileEmitter::Control::CONTROLLABLE;
  const char* keyField           = Lua::Level::GameObject::Components::ProjectileEmitter::Control::KEY;

  sol::optional<bool> controllableOpt = ProjEmitterControlTable[controllableField];
  sol::optional<char> keyOpt          = ProjEmitterControlTable[keyField];

  SDL_Keycode key = Defaults::ProjectileEmitter::CONTROL_KEY;

  if(controllableOpt != sol::nullopt && controllableOpt.value()){
    if(keyOpt != sol::nullopt){
      key = static_cast<SDL_Keycode>(keyOpt.value());  //NOLINT
    }

    gameObject.addComponent<ProjectileControlComponent>(key);
    spdlog::info("in File: '{}': Attaching Project Control to gameObject id '{}'", m_path, gameObject.id());
  }

};


/* */

void LevelManager::doKeyboardControl(sol::table &keyControlTable, GameObject gameObject) const{

  const char * upField       = Lua::Level::GameObject::Components::KeyboardControl::UP_VELOCITY;
  const char * rightField    = Lua::Level::GameObject::Components::KeyboardControl::RIGHT_VELOCITY;
  const char * downField     = Lua::Level::GameObject::Components::KeyboardControl::DOWN_VELOCITY;
  const char * leftField     = Lua::Level::GameObject::Components::KeyboardControl::LEFT_VELOCITY;
  const char * strategyField = Lua::Level::GameObject::Components::KeyboardControl::STRATEGY;

  sol::optional<double> upOpt             = keyControlTable[upField];
  sol::optional<double> rightOpt          = keyControlTable[rightField];
  sol::optional<double> downOpt           = keyControlTable[downField];
  sol::optional<double> leftOpt           = keyControlTable[leftField];
  sol::optional<std::string> strategyOpt  = keyControlTable[strategyField];

  using Strategy = KeyboardControlComponent::Strategy;

  double up    = Defaults::KeyboardControl::UP_VELOCITY; // NOLINT
  double right = Defaults::KeyboardControl::RIGHT_VELOCITY;
  double down  = Defaults::KeyboardControl::DOWN_VELOCITY;
  double left  = Defaults::KeyboardControl:: LEFT_VELOCITY;
  auto strategy  = static_cast<Strategy>(Defaults::KeyboardControl::STRATEGY);


  if(upOpt != sol::nullopt){
    up = upOpt.value();
  } else {
    spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, upField, gameObject.id());
  }

  if(rightOpt != sol::nullopt){
    right = rightOpt.value();
  } else {
    spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, rightField, gameObject.id());
  }

  if(downOpt != sol::nullopt){
    down =  downOpt.value();
  } else {
    spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, downField, gameObject.id());
  }

  if(leftOpt != sol::nullopt){
    left = leftOpt.value();
  } else {
    spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, leftField, gameObject.id());
  }

  if(strategyOpt != sol::nullopt){

    auto strategyOptions = Lua::Level::GameObject::Components::KeyboardControl::STRATEGY_OPS;

    size_t pos = std::distance(strategyOptions.begin(),
        std::find(strategyOptions.begin(), strategyOptions.end(), strategyOpt.value()));

    if(pos < strategyOptions.size()){
         strategy = static_cast<Strategy>(pos);
        // spdlog::error("--> {}", strategy);
    } else {
         spdlog::warn("In File: '{}': Invalid '{}' options. Must be {}. Loading defaults.", m_path, strategyField,
         Lua::Level::GameObject::Components::KeyboardControl::STRATEGY_MESSAGE);
    }

  } else {
    spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, strategyField, gameObject.id());
  }

  gameObject.addComponent<KeyboardControlComponent>(up, right, down, left, strategy);
}

/* */

void LevelManager::doCameraFollow(sol::table &camFollowTable, GameObject gameObject) const {
    const char* followField = Lua::Level::GameObject::Components::CameraFollow::FOLLOW;


    sol::optional<bool> followOpt = camFollowTable[followField];

    if(followOpt != sol::nullopt){
        if(followOpt.value()){
           gameObject.addComponent<CameraFollowerComponent> ();
      }
    } else {
     spdlog::warn("In File: '{}': '{}' field not present. Loading default for GameObject id '{}'.", m_path, followField, gameObject.id());

    }
}

auto LevelManager::doPadding(sol::optional<sol::table> &paddingOpt) const -> Padding {

  if(paddingOpt == sol::nullopt){
    spdlog::warn( "In file '{}': Using default padding top = '{}' , right = '{}', bottom = '{}', right = '{}'.",
        m_path,
        Defaults::Map::BoundsPadding::PADDING.top,
        Defaults::Map::BoundsPadding::PADDING.right,
        Defaults::Map::BoundsPadding::PADDING.bottom,
        Defaults::Map::BoundsPadding::PADDING.left
        );

    return constants::Defaults::Map::BoundsPadding::PADDING;
  }

  sol::optional<float> topOpt = paddingOpt.value()[Lua::Level::Map::Padding::TOP];
  sol::optional<float> rightOpt = paddingOpt.value()[Lua::Level::Map::Padding::RIGHT];
  sol::optional<float> bottomOpt = paddingOpt.value()[Lua::Level::Map::Padding::BOTTOM];
  sol::optional<float> leftOpt = paddingOpt.value()[Lua::Level::Map::Padding::LEFT];

  Padding padding;

  padding.top = topOpt == sol::nullopt ? Defaults::Map::BoundsPadding::PADDING.top : topOpt.value();
  warn(topOpt, "Padding::" + std::string(Lua::Level::Map::Padding::TOP), Defaults::Map::BoundsPadding::PADDING.top);

  padding.top = rightOpt == sol::nullopt ? Defaults::Map::BoundsPadding::PADDING.right : rightOpt.value();
  warn(rightOpt, "Padding::" + std::string(Lua::Level::Map::Padding::RIGHT), Defaults::Map::BoundsPadding::PADDING.right);

  padding.top = bottomOpt == sol::nullopt ? Defaults::Map::BoundsPadding::PADDING.bottom : bottomOpt.value();
  warn(bottomOpt, "Padding::" + std::string(Lua::Level::Map::Padding::BOTTOM), Defaults::Map::BoundsPadding::PADDING.bottom);

  padding.top = leftOpt == sol::nullopt ? Defaults::Map::BoundsPadding::PADDING.left : leftOpt.value();
  warn(leftOpt, "Padding::" + std::string(Lua::Level::Map::Padding::LEFT), Defaults::Map::BoundsPadding::PADDING.left);

  return padding;
}

/* Helpers */

auto LevelManager::doWidthHeight(sol::table &widthHeightTable, glm::vec2 defaultValue, const char *tableKey) const -> glm::vec2{
  glm::vec2 vec                    = defaultValue;

  sol::optional<int32_t> widthOpt = widthHeightTable[Lua::GenericTable::Size::WIDTH];
  sol::optional<int32_t> heightOpt = widthHeightTable[Lua::GenericTable::Size::HEIGHT];

  if(widthOpt != sol::nullopt){
    vec.x =  static_cast<float>(widthOpt.value()); //NOLINT
  } else {
    spdlog::warn("In File: '{}': Missing 'width' key in '{}' . Loading default.", m_path, tableKey);
  }

  if(heightOpt != sol::nullopt){
    vec.y =  static_cast<float>(heightOpt.value()); //NOLINT
  } else {
    spdlog::warn("In File: '{}': Missing 'height' key value in '{}' . Loading default.", m_path, tableKey);
  }

  return vec;
}

auto LevelManager::doCrop(sol::table &cropTable, SDL_Rect defaultValue, const char *tableKey) const -> SDL_Rect{

  SDL_Rect rect                    = defaultValue;

  sol::optional<int32_t> xOpt =   cropTable[Lua::GenericTable::XY::X];
  sol::optional<int32_t> yOpt =   cropTable[Lua::GenericTable::XY::Y];
  sol::optional<int32_t> widthOpt = cropTable[Lua::GenericTable::Size::WIDTH];
  sol::optional<int32_t> heightOpt = cropTable[Lua::GenericTable::Size::HEIGHT];

  if(xOpt != sol::nullopt){
    rect.x =  static_cast<float>(xOpt.value()); //NOLINT
  } else {
    spdlog::warn("In File: '{}': Missing 'x' key in '{}' . Loading default.", m_path, tableKey);
  }

  if(yOpt != sol::nullopt){
    rect.y =  static_cast<float>(yOpt.value()); //NOLINT
  } else {
    spdlog::warn("In File: '{}': Missing 'y' key value in '{}' . Loading default.", m_path, tableKey);
  }

  if(widthOpt != sol::nullopt){
    rect.w =  static_cast<float>(widthOpt.value()); //NOLINT
  } else {
    spdlog::warn("In File: '{}': Missing 'width' key in '{}' . Loading default.", m_path, tableKey);
  }

  if(heightOpt != sol::nullopt){
    rect.h =  static_cast<float>(heightOpt.value()); //NOLINT
  } else {
    spdlog::warn("In File: '{}': Missing 'height' key value in '{}' . Loading default.", m_path, tableKey);
  }

  return rect;

}
