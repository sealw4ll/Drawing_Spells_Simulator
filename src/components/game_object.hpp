#pragma once

#include <entt/entt.hpp>
#include <raymath.h>

struct GameObjects {
	Vector2 mouse = { 0, 0 };
	float screenWidth = 800;
	float screenHeight = 600;
	Vector2 centerScreen = { 400, 300 };
	entt::registry registry;
};