#pragma once

// lib includes
#include <raylib.h>
#include <iostream>
#include <entt/entt.hpp>
#include <vector>
#include <cmath>
#include <bitset>
#include <utility>
#include <thread>
#include <chrono>

// config
#include "config.hpp"

// component includes
#include "components/camera.hpp"
#include "components/dot.hpp"
#include "components/game_object.hpp"
#include "components/player.hpp"
#include "components/spell.hpp"
#include "components/enemy.hpp"

// systems includes
#include "systems/spell_loader.hpp"
#include "systems/drawing_module.hpp"
#include "systems/camera_system.hpp"
#include "systems/spell_system.hpp"
#include "systems/player_system.hpp"
#include "systems/enemy_system.hpp"

// helpers
#include "systems/helpers/init_utils.hpp"
