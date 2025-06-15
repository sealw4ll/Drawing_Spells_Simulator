#pragma once

#include <raylib.h>
#include <tuple>
#include <cmath>
#include "rendering_utils.hpp"

bool isPointInCircle(Vector2 point, Vector2 center, float radius);
bool isPointInRectangle(Vector2 point, Vector2 center, float width, float height);
bool isPointNearLine(Vector2 point, Vector2 a, Vector2 b, float thickness);
bool isPointInTriangle(Vector2 point, Vector2 start, Vector2 end, float size);
std::tuple<Vector2, Vector2, Vector2> getIsoTrianglePoints(Vector2 start, Vector2 end, float size);
