#include "rendering_utils.hpp"

Vector2 isoToScreen(float x, float y) {
	return {
		(x - y) * TILE_WIDTH / 2.0f,
		(x + y) * TILE_HEIGHT / 2.0f
	};
}

Vector2 screenToIso(float screenX, float screenY) {
	float x = (screenY / (TILE_HEIGHT / 2) + screenX / (TILE_WIDTH / 2)) / 2.0f;
	float y = (screenY / (TILE_HEIGHT / 2) - screenX / (TILE_WIDTH / 2)) / 2.0f;
	return { x, y };
}

void drawIsoTile(float x, float y, Color color) {
	Vector2 center = isoToScreen(x, y);
	Vector2 top = { center.x, center.y - TILE_HEIGHT / 2.0f };
	Vector2 right = { center.x + TILE_WIDTH / 2.0f, center.y };
	Vector2 bottom = { center.x, center.y + TILE_HEIGHT / 2.0f };
	Vector2 left = { center.x - TILE_WIDTH / 2.0f, center.y };

	DrawLineV(top, right, color);
	DrawLineV(right, bottom, color);
	DrawLineV(bottom, left, color);
	DrawLineV(left, top, color);
}
