#include "main.hpp"

// Tile size
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 16;

// Grid size
const int MAP_WIDTH = 10000;
const int MAP_HEIGHT = 10000;

const float MAP_SCALE = 5;

void writeCastedSpell(GameObjects& game,std::optional<entt::entity> spellEntity){
	std::string s;
	if (!spellEntity) {
		DrawText("Spell not found", 10, 10, 20, RED);
		return;
	}
	auto& spellInfo = game.registry.get<Spell>(*spellEntity);
	s = "Spell casted: " + spellInfo.name;
	DrawText(s.c_str(), 10, 10, 20, BLACK);
}

static void initGameObjects(GameObjects& game){
	//game.screenWidth = static_cast<float>(GetScreenWidth());
	//game.screenHeight = static_cast<float>(GetScreenHeight());
	game.centerScreen = Vector2{ game.screenWidth / 2.0f, game.screenHeight / 2.0f };
}

entt::entity initPlayer(GameObjects& game){
	auto entity = game.registry.create();
	Player player;
	player.position = { MAP_WIDTH / 2.0f, MAP_HEIGHT / 2.0f };
	game.registry.emplace<Player>(entity, std::move(player));
	return entity;
}

Vector2 isoToScreenVector(Vector2 pos) {
	return {
		(pos.x - pos.y) * TILE_WIDTH / 2 + 400,
		(pos.x + pos.y) * TILE_HEIGHT / 2 + 100
	};
}

Vector2 screenToIso(float screenX, float screenY) {
	float x = (screenY / (TILE_HEIGHT / 2) + screenX / (TILE_WIDTH / 2)) / 2.0f;
	float y = (screenY / (TILE_HEIGHT / 2) - screenX / (TILE_WIDTH / 2)) / 2.0f;
	return { x, y };
}

Vector2 isoToScreenInt(int x, int y) {
	return {
		static_cast<float>((x - y) * TILE_WIDTH / 2 + 400),
		static_cast<float>((x + y) * TILE_HEIGHT / 2 + 100)
	};
}

entt::entity initCamera(GameObjects& game, Player player) {
	CameraObject module;

	module.camera.target = { player.position.x, player.position.y };
	module.camera.offset = { game.screenWidth / 2.0f, game.screenHeight / 2.0f };
	module.camera.zoom = 1.0f;
	module.camera.rotation = 0.0f;

	auto entity = game.registry.create();
	game.registry.emplace<CameraObject>(entity, std::move(module));
	return entity;
}

Vector2 isoToScreen(float x, float y) {
	return {
		(x - y) * TILE_WIDTH / 2.0f,
		(x + y) * TILE_HEIGHT / 2.0f
	};
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

void drawPill(Vector2 center, float width, float height, Color fill, Color outline, float outlineThickness = 3.0f) {
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

//void drawPill(Vector2 isoCenter, float width, float height, Color fill, Color outline, float rotationRadians = 0.0f) {
//	const int segments = 40;
//
//	// Center in screen space
//	Vector2 center = isoToScreen(isoCenter.x, isoCenter.y);
//	Vector2 points[segments + 1];
//
//	for (int i = 0; i <= segments; ++i) {
//		float angle = (-2 * PI * i / segments);  // Base angle
//
//		// Elliptical shape before rotation
//		float dx = cosf(angle) * width;
//		float dy = sinf(angle) * height;
//
//		// Convert to screen space first
//		Vector2 screenPoint = isoToScreen(isoCenter.x + dx, isoCenter.y + dy);
//
//		// Rotate around the screen-space center
//		float rotatedX = (screenPoint.x - center.x) * cosf(rotationRadians) - (screenPoint.y - center.y) * sinf(rotationRadians);
//		float rotatedY = (screenPoint.x - center.x) * sinf(rotationRadians) + (screenPoint.y - center.y) * cosf(rotationRadians);
//
//		// Shift back to original position
//		points[i] = { center.x + rotatedX, center.y + rotatedY };
//	}
//
//	// Fill using triangle fan
//	for (int i = 0; i < segments; ++i) {
//		DrawTriangle(center, points[i], points[i + 1], fill);
//	}
//
//	// Outline
//	for (int i = 0; i < segments; ++i) {
//		DrawLineV(points[i], points[i + 1], outline);
//	}
//}


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

std::optional<Vector2> handleSpellTargeting(GameObjects& game, Camera2D camera, std::optional<entt::entity> spell, entt::entity& drawingModule, Vector2 playerIsoPos) {
	if (!spell || game.registry.get<DrawingModule>(drawingModule).isVisible == true) return std::nullopt;

	float spellRange = game.registry.get<SpellStats>(*spell).range;
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
	//if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		Vector2 mouseScreen = GetMousePosition();
		Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
		Vector2 clickedTile = screenToIso(mouseWorld.x, mouseWorld.y);
		//std::cout << "clicked: " << clickedTile.x << " | " << clickedTile.y << "\n";

		// Distance in isometric "grid units"
		Vector2 delta = {
			clickedTile.x - playerIsoPos.x,
			clickedTile.y - playerIsoPos.y
		};

		float distance = sqrtf(delta.x * delta.x + delta.y * delta.y) * MAP_SCALE;


		std::cout << "distance: " << distance << " | " << "range: " << spellRange << "\n";
		std::cout << "clicked: " << clickedTile.x << ", " << clickedTile.y << " | " << "playerpos: " << playerIsoPos.x << ", " << playerIsoPos.y << "\n";
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

void drawSpellRangeCircle(GameObjects &game, Vector2 playerPos, std::optional<entt::entity> spell, Color color) {
	if (!spell) return;

	std::cout << "AAAAA" << "\n";
	float isoRadius = game.registry.get<SpellStats>(*spell).range / MAP_SCALE;
	const int segments = 64;

	for (int i = 0; i < segments; ++i) {
		float angle1 = (2 * PI * i) / segments;
		float angle2 = (2 * PI * (i + 1)) / segments;

		// Point in isometric "circular" radius space
		float isoX1 = cosf(angle1) * isoRadius;
		float isoY1 = sinf(angle1) * isoRadius;
		float isoX2 = cosf(angle2) * isoRadius;
		float isoY2 = sinf(angle2) * isoRadius;

		// Project those isometric coords into screen space
		Vector2 p1 = isoToScreen(playerPos.x + isoX1, playerPos.y + isoY1);
		Vector2 p2 = isoToScreen(playerPos.x + isoX2, playerPos.y + isoY2);

		DrawLineV(p1, p2, color);
	}
	DrawCircleV(isoToScreen(playerPos.x, playerPos.y), 5, BLACK);
}


void drawSpells(GameObjects& game) {
	auto spellView = game.registry.view<Spell, SpellShape, SpellStats>();

	for (auto entity : spellView) {
		auto& spell = game.registry.get<Spell>(entity);
		auto& shape = game.registry.get<SpellShape>(entity);
		auto& stats = game.registry.get<SpellStats>(entity);

		Vector2 screenPos = isoToScreen(spell.position.x, spell.position.y);

		switch (shape.type) {
		case ShapeType::CIRCLE:
			DrawCircleV(screenPos, shape.size * MAP_SCALE, RED);
			break;
		case ShapeType::SQUARE:
			DrawRectangleV({ screenPos.x - shape.size, screenPos.y - shape.size }, { shape.size * 2, shape.size * 2 }, RED);
			break;
		case ShapeType::CONE:
			DrawTriangle({ screenPos.x, screenPos.y }, { screenPos.x + shape.size, screenPos.y - shape.size }, { screenPos.x - shape.size, screenPos.y - shape.size }, RED);
			break;
		case ShapeType::LINE:
			DrawLineEx(screenPos, { screenPos.x + shape.size, screenPos.y }, 5, RED);
			break;
		default:
			break;
		}

		// Handle spell expiration
		if (--stats.duration <= 0) {
			game.registry.destroy(entity);
		}
	}
}


int main() {
	GameObjects game;

	//SetConfigFlags(FLAG_FULLSCREEN_MODE);
	initGameObjects(game);
	InitWindow(game.screenWidth, game.screenHeight, "Drawing Spells Simulator");
	SetTargetFPS(60);

	loadSpells(game.registry, "../spells/");

	entt::entity playerEntity = initPlayer(game);
	Player player = game.registry.get<Player>(playerEntity);

	entt::entity cameraEntity = initCamera(game, player);
	entt::entity drawingModule = createDrawingModule(game);
	std::optional<entt::entity> spellCasted = std::nullopt;

	std::optional<Vector2> spellTarget;
	std::optional <entt::entity> activeSpell = std::nullopt; // Ensure only one spell at a time
	//std::vector<entt::entity> activeSpells;
	while (!WindowShouldClose()) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
		// --- Input ---
		if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
			player.position.x -= 0.1f;
			player.position.y -= 0.1f;
		};
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
			player.position.x -= 0.1f;
			player.position.y += 0.1f;
		}
		if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
			player.position.x += 0.1f;
			player.position.y += 0.1f;
		};
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
			player.position.x += 0.1f;
			player.position.y -= 0.1f;
		};
		if (player.position.x <= 0) player.position.x = 0;
		if (player.position.y <= 0) player.position.y = 0;
		if (player.position.x >= MAP_WIDTH) player.position.x = MAP_WIDTH;
		if (player.position.y >= MAP_HEIGHT) player.position.y = MAP_HEIGHT;
		game.mouse = GetMousePosition();

		// holt to open drawing module
		if (IsKeyPressed(KEY_LEFT_CONTROL)) {
			toggleDrawingModule(game, drawingModule);
		}

		// Release to close drawing module and cast spell
		if (IsKeyReleased(KEY_LEFT_CONTROL)) {
			spellCasted = printAndResetConnections(game, drawingModule);
			if (spellCasted)
				std::cout << "what" << "\n";
			toggleDrawingModule(game, drawingModule);
			spellTarget.reset();
		}

		drawingModuleDotsCheck(game, drawingModule);

		BeginDrawing();
		ClearBackground(DARKGREEN);

		auto& camera = game.registry.get<CameraObject>(cameraEntity).camera;
		BeginMode2D(camera);
		drawVisibleTiles(game, camera);

		// Draw player cube
		Vector2 playerPosIso = isoToScreen(player.position.x, player.position.y);
		updateCamera(game, playerPosIso, cameraEntity);
		drawSpellRangeCircle(game, player.position, spellCasted, BLACK);

		std::optional<Vector2> spellTarget = handleSpellTargeting(game, camera, spellCasted, drawingModule, player.position);
		if (spellTarget) {
			if (activeSpell) {
				game.registry.destroy(*activeSpell);
				activeSpell.reset();
			}

			// Create new spell entity
			entt::entity spellEntity = game.registry.create();
			game.registry.emplace<Spell>(spellEntity, game.registry.get<Spell>(*spellCasted)); // Copy spell data
			game.registry.emplace<SpellStats>(spellEntity, game.registry.get<SpellStats>(*spellCasted));
			game.registry.emplace<SpellShape>(spellEntity, game.registry.get<SpellShape>(*spellCasted));
			game.registry.get<Spell>(spellEntity).position = *spellTarget; // Assign position

			activeSpell = spellEntity;
		}
		//drawVerticalIsometricPill(player.position, 0.5, 1, RED, BLACK);
		drawPill(playerPosIso, 16, 24, RED, BLACK);
		EndMode2D();

		drawSpells(game);
		writeCastedSpell(game, spellCasted);
		renderDrawingModule(game, drawingModule);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
