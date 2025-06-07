#pragma once

// lib includes
#include <iostream>
#include <raylib.h>
#include <entt/entt.hpp>
#include <vector>
#include <cmath>
#include <bitset>
#include <utility>
#include <raymath.h>

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