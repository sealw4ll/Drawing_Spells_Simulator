#pragma once

#include <raylib.h>
#include "../../config.hpp"
#include "../../components/camera.hpp"
#include "../../components/game_object.hpp"

Vector2 isoToScreen(float x, float y);
Vector2 screenToIso(float screenX, float screenY);
void drawIsoTile(float x, float y, Color color);
void drawPill(Vector2 center, float width, float height, Color fill, Color outline, float outlineThickness = 2.0f);
void drawVisibleTiles(GameObjects& game, Camera2D camera);