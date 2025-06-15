#include "spell_system.hpp"


// DRAWING
void hitEnemy(GameObjects &game, SpellActiveComponent& activeComponent, SpellShape& shape, SpellStats& stats) {
	if (activeComponent.hit == true) return ;
	auto enemyView = game.registry.view<Enemy, EnemyActiveComponent>();
	for (auto enemyEntity : enemyView) {
		auto& enemy = game.registry.get<Enemy>(enemyEntity);
		auto& enemyActive = game.registry.get<EnemyActiveComponent>(enemyEntity);

		Vector2 enemyPos = enemy.position;

		bool hit = false;

		switch (shape.type) {
		case ShapeType::CIRCLE:
			hit = isPointInCircle(enemyPos, activeComponent.position, shape.size / MAP_SCALE);
			break;
		case ShapeType::TARGET:
			hit = isPointInCircle(enemyPos, activeComponent.position, 5 / MAP_SCALE);
			break;
		case ShapeType::SQUARE:
			hit = isPointInRectangle(enemyPos, activeComponent.position, shape.size / MAP_SCALE, shape.size / MAP_SCALE);
			break;
		case ShapeType::LINE:
			hit = isPointNearLine(enemyPos, activeComponent.positionCasted, activeComponent.position, shape.size / MAP_SCALE);
			break;
		case ShapeType::CONE:
			hit = isPointInTriangle(enemyPos, activeComponent.positionCasted, activeComponent.position, shape.size / MAP_SCALE);
			break;
		}

		if (hit) {
			activeComponent.hit = true;
			// Apply damage
			enemyActive.currentHp -= stats.damage;

			// Remove enemy if dead
			if (enemyActive.currentHp <= 0) {
				game.registry.destroy(enemyEntity);
			}
		}
	}
}

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

		hitEnemy(game, activeComponent, shape, stats);

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

	std::tuple<Vector2, Vector2, Vector2> trianglePoints = getIsoTrianglePoints(start, end, size);
	auto [screenStart, screenV2, screenV3] = trianglePoints;

	// Draw filled triangle
	DrawTriangle(screenStart, screenV2, screenV3, color);
}

void writeCastedSpell(GameObjects& game, std::optional<entt::entity> spellEntity) {
	std::string s;
	if (!spellEntity) {
		DrawText("Spell not found (Hold left ctrl to cast spells)", 10, 10, 20, RED);
		return;
	}
	auto activeSpellEntity = game.registry.view<SpellActiveComponent>();
	auto& spellInfo = game.registry.get<Spell>(*spellEntity);
	s = "Click to cast: " + spellInfo.name;
	DrawText(s.c_str(), 10, 10, 20, BLACK);
}


// LOGIC

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

void activeSpellHandler(GameObjects& game, std::optional<entt::entity>& activeSpell, std::optional<Vector2>& spellTarget, std::optional<entt::entity>& spellCasted, Vector2 playerPos) {
	if (activeSpell) {
		game.registry.remove<SpellActiveComponent>(*activeSpell);
		activeSpell.reset();
	}

	SpellActiveComponent module;
	module.position = *spellTarget;
	module.positionCasted = playerPos;
	module.lifeTime = game.registry.get<SpellStats>(*spellCasted).duration;
	game.registry.emplace<SpellActiveComponent>(*spellCasted, std::move(module));

	activeSpell = *spellCasted;
	spellTarget = std::nullopt;
	spellCasted = std::nullopt;
}