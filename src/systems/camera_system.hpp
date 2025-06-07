#pragma once

#include <raylib.h>
#include <raymath.h>
#include <entt/entt.hpp>
#include "../components/camera.hpp"
#include "../components/game_object.hpp"

void updateCamera(GameObjects& game, Vector2 playerPosition, entt::entity cameraEntity);