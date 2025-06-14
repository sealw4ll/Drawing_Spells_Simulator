#pragma once

#include <raylib.h>
#include "../../config.hpp"
Vector2 isoToScreen(float x, float y);
Vector2 screenToIso(float screenX, float screenY);
void drawIsoTile(float x, float y, Color color);