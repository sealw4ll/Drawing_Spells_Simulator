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

void drawPill(Vector2 center, float width, float height, Color fill, Color outline, float outlineThickness) {
	float halfW = width / 2.0f;
	float halfH = height / 2.0f;

	// Draw outline
	DrawRectangleRounded(
		{ center.x - halfW - outlineThickness, center.y - halfH - outlineThickness, width + 2 * outlineThickness, height + 2 * outlineThickness },
		0.5f,
		16,
		outline
	);

	// Center rectangle
	DrawRectangleRounded(
		{ center.x - halfW, center.y - halfH, width, height },
		0.5f,
		16,
		fill
	);
}


void drawVisibleTiles(GameObjects& game, Camera2D camera) {
	Vector2 topLeft = GetScreenToWorld2D({ 0, 0 }, camera);
	Vector2 topRight = GetScreenToWorld2D({ game.screenWidth, 0 }, camera);
	Vector2 bottomLeft = GetScreenToWorld2D({ 0, game.screenHeight }, camera);
	Vector2 bottomRight = GetScreenToWorld2D({ game.screenWidth, game.screenHeight }, camera);

	Vector2 isoTL = screenToIso(topLeft.x, topLeft.y);
	Vector2 isoTR = screenToIso(topRight.x, topRight.y);
	Vector2 isoBL = screenToIso(bottomLeft.x, bottomLeft.y);
	Vector2 isoBR = screenToIso(bottomRight.x, bottomRight.y);

	// Get overall min/max from all four corners
	float minXf = fminf(fminf(isoTL.x, isoTR.x), fminf(isoBL.x, isoBR.x));
	float maxXf = fmaxf(fmaxf(isoTL.x, isoTR.x), fmaxf(isoBL.x, isoBR.x));
	float minYf = fminf(fminf(isoTL.y, isoTR.y), fminf(isoBL.y, isoBR.y));
	float maxYf = fmaxf(fmaxf(isoTL.y, isoTR.y), fmaxf(isoBL.y, isoBR.y));

	// Convert to integer tile bounds + padding
	int minX = (int)floor(minXf) - 2;
	int maxX = (int)ceil(maxXf) + 2;
	int minY = (int)floor(minYf) - 2;
	int maxY = (int)ceil(maxYf) + 2;

	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT) continue;
			drawIsoTile(x, y, GRAY);
		}
	}
}