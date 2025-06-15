#pragma once

#include <raylib.h>
#include <entt/entt.hpp>

#include "../config.hpp";
#include "../components/enemy.hpp"
#include "../components/player.hpp"
#include "../components/game_object.hpp"
#include "helpers/rendering_utils.hpp"

static float getRandomFloat(float min, float max);
void enemySpawnSystem(GameObjects& game, float spawnInterval = 8);
void drawHpBar(Vector2 position, float hp, float maxHp, float width, float height, Color fill, Color outline);
void renderEnemies(GameObjects& game);