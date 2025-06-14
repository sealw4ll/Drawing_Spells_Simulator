#include "spell_system.hpp"

void drawSpells(GameObjects& game, Vector2 playerPosScreen) {
	auto spellView = game.registry.view<Spell, SpellShape, SpellStats, SpellActiveComponent, SpellIdentifier>();

	for (auto entity : spellView) {
		auto& spell = game.registry.get<Spell>(entity);
		auto& shape = game.registry.get<SpellShape>(entity);
		auto& stats = game.registry.get<SpellStats>(entity);
		auto& identifier = game.registry.get<SpellIdentifier>(entity);
		auto& activeComponent = game.registry.get<SpellActiveComponent>(entity);

		Vector2 spellScreenPos = isoToScreen(activeComponent.position.x, activeComponent.position.y);

		switch (shape.type) {
		case ShapeType::CIRCLE:
			drawIsoCircle(activeComponent.position, shape.size / MAP_SCALE, damageTypeToColor(identifier.damageType));
			break;
		case ShapeType::TARGET:
			drawIsoCircle(activeComponent.position, 5 / MAP_SCALE, damageTypeToColor(identifier.damageType));
			break;
		case ShapeType::SQUARE:
			drawIsoRectangle(activeComponent.position, shape.size / MAP_SCALE, shape.size / MAP_SCALE, damageTypeToColor(identifier.damageType));
			break;
		case ShapeType::CONE:
			drawIsoTriangle(activeComponent.positionCasted, activeComponent.position, shape.size / MAP_SCALE, damageTypeToColor(identifier.damageType));
			break;
		case ShapeType::LINE:
			drawIsoLine(activeComponent.positionCasted, activeComponent.position, shape.size * MAP_SCALE, damageTypeToColor(identifier.damageType));
			break;
		default:
			break;
		}

		//Handle spell expiration
		if (--activeComponent.lifeTime <= 0) {
			game.registry.remove<SpellActiveComponent>(entity);
		}
	}
}

void drawIsoCircle(Vector2 center, float radius, Color color, const bool fill) {
	const int segments = 64;
	Vector2 screenCenter = isoToScreen(center.x, center.y);
	Vector2 points[segments];

	// Compute isometric circle points
	for (int i = 0; i < segments; ++i) {
		float angle = (2 * PI * i) / segments;
		float isoX = cosf(angle) * radius;
		float isoY = sinf(angle) * radius;

		points[i] = isoToScreen(center.x + isoX, center.y + isoY);
	}

	// Fill the circle using a triangle fan if fill is true
	if (fill) {
		for (int i = 0; i < segments - 1; ++i) {
			DrawTriangle(points[i + 1], points[i], screenCenter, color);
		}
		// Final triangle to close the circle
		DrawTriangle(points[0], points[segments - 1], screenCenter, color);
	}

	// Draw the circle outline
	for (int i = 0; i < segments - 1; ++i) {
		DrawLineV(points[i], points[i + 1], color);
	}
	DrawLineV(points[segments - 1], points[0], color);
}

void drawSpellRangeCircle(GameObjects& game, Vector2 playerPos, std::optional<entt::entity> spell, Color color) {
	if (!spell) return;

	float isoRadius = game.registry.get<SpellStats>(*spell).range / MAP_SCALE;
	drawIsoCircle(playerPos, isoRadius, color, false);
	DrawCircleV(isoToScreen(playerPos.x, playerPos.y), 5, BLACK);
}

void drawIsoRectangle(Vector2 center, float width, float height, Color color) {
	// Compute the four isometric corners
	Vector2 topLeft = isoToScreen(center.x - width / 2, center.y - height / 2);
	Vector2 topRight = isoToScreen(center.x + width / 2, center.y - height / 2);
	Vector2 bottomLeft = isoToScreen(center.x - width / 2, center.y + height / 2);
	Vector2 bottomRight = isoToScreen(center.x + width / 2, center.y + height / 2);

	// Draw filled rectangle
	DrawTriangle(topLeft, bottomLeft, topRight, color);
	DrawTriangle(topRight, bottomLeft, bottomRight, color);
}

void drawIsoLine(Vector2 start, Vector2 end, float thickness, Color color) {
	// Convert both start and end points to isometric screen space
	Vector2 screenStart = isoToScreen(start.x, start.y);
	Vector2 screenEnd = isoToScreen(end.x, end.y);

	// Draw the line in isometric space
	DrawLineEx(screenStart, screenEnd, thickness, color);
}

void drawIsoTriangle(Vector2 start, Vector2 end, float size, Color color) {
	// Convert isometric coordinates to screen space
	Vector2 screenStart = isoToScreen(start.x, start.y);

	// Direction vector in isometric space
	Vector2 dir = { end.x - start.x, end.y - start.y };
	float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
	if (length == 0) return; // Avoid divide by zero

	// Normalize direction vector
	dir.x /= length;
	dir.y /= length;

	// Calculate the isometric point at the tip of the triangle (height = size)
	Vector2 isoTip = {
		start.x + dir.x * size,
		start.y + dir.y * size
	};
	Vector2 screenTip = isoToScreen(isoTip.x, isoTip.y);

	// Perpendicular vector for base (rotated 90°)
	Vector2 perp = { -dir.y, dir.x };

	// Base half-width
	float halfWidth = size / 2.0f;

	// Points for base of the triangle in isometric space
	Vector2 isoV2 = {
		isoTip.x + perp.x * halfWidth,
		isoTip.y + perp.y * halfWidth
	};
	Vector2 isoV3 = {
		isoTip.x - perp.x * halfWidth,
		isoTip.y - perp.y * halfWidth
	};

	Vector2 screenV2 = isoToScreen(isoV2.x, isoV2.y);
	Vector2 screenV3 = isoToScreen(isoV3.x, isoV3.y);

	// Draw filled triangle
	DrawTriangle(screenStart, screenV2, screenV3, color);
}

std::optional<Vector2> handleSpellTargeting(GameObjects& game, Camera2D camera, std::optional<entt::entity> spell, entt::entity& drawingModule, Vector2 playerIsoPos) {
	if (!spell || game.registry.get<DrawingModule>(drawingModule).isVisible == true) return std::nullopt;

	float spellRange = game.registry.get<SpellStats>(*spell).range;
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Vector2 mouseScreen = GetMousePosition();
		Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
		Vector2 clickedTile = screenToIso(mouseWorld.x, mouseWorld.y);

		// Distance in isometric "grid units"
		Vector2 delta = {
			clickedTile.x - playerIsoPos.x,
			clickedTile.y - playerIsoPos.y
		};

		float distance = sqrtf(delta.x * delta.x + delta.y * delta.y) * MAP_SCALE;

		// If in range, return it
		if (distance <= spellRange) return clickedTile;


		// Otherwise clamp to edge of circle
		float scale = spellRange / distance;

		Vector2 clamped = {
			playerIsoPos.x + delta.x * scale,
			playerIsoPos.y + delta.y * scale
		};

		return clamped;
	}
	return std::nullopt;
}