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

// component includes
#include "components/camera.hpp"
#include "components/dot.hpp"
#include "components/game_object.hpp"
#include "components/player.hpp"
#include "components/spell.hpp"

// systems includes
#include "systems/spell_loader.hpp"
#include "systems/drawing_module.hpp"
#include "systems/camera_system.hpp"