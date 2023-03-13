#ifndef ALL_SYSTEMS_H
#define ALL_SYSTEMS_H


#include "systems/movement_system.hpp"
#include "systems/animation_system.hpp"
#include "systems/collision_system.hpp"
#include "systems/damage_system.hpp"
#include "systems/keyboard_control_system.hpp"
#include "systems/camera_movement_system.hpp"

/* Projectile */
#include "systems/projectile/projectile_emit_system.hpp"
#include "systems/projectile/projectile_lifecycle_system.hpp"
#include "systems/projectile/projectile_control_system.hpp"

/* Render */
#include "systems/render/render_sprite_system.hpp"
#include "systems/render/render_debug_system.hpp"
#include "systems/render/render_text_system.hpp"
#include "systems/render/render_healthbar_system.hpp"

/* GUI */
#include "systems/gui/render_gui_system.hpp"


#endif
