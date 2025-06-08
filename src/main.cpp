#include "main.hpp"

// Tile size
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 16;

// Grid size
const int MAP_WIDTH = 10000;
const int MAP_HEIGHT = 10000;

void drawCastedSpell(GameObjects& game,std::optional<entt::entity> spellEntity){
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

void drawPill(Vector2 center, float width, float height, Color color) {
	float halfW = width / 2.0f;
	float halfH = height / 2.0f;

	// Center rectangle
	DrawRectangleRounded(
		{ center.x - halfW, center.y - halfH, width, height },
		0.5f,  // Roundness (0.0 to 1.0)
		16,    // segments
		color
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

std::optional<Vector2> handleSpellTargeting(GameObjects& game, Camera2D camera, std::optional<entt::entity> spell, entt::entity& drawingModule, Vector2 playerIsoPos) {
	if (!spell || game.registry.get<DrawingModule>(drawingModule).isVisible == true) return std::nullopt;

	float spellRange = game.registry.get<SpellStats>(*spell).range;
	//if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		Vector2 mouseScreen = GetMousePosition();
		Vector2 mouseWorld = GetScreenToWorld2D(mouseScreen, camera);
		Vector2 clickedTile = screenToIso(mouseWorld.x, mouseWorld.y);
		//std::cout << "clicked: " << clickedTile.x << " | " << clickedTile.y << "\n";

		// Distance in isometric "grid units"
		Vector2 delta = {
			clickedTile.x - playerIsoPos.x,
			clickedTile.y - playerIsoPos.y
		};

		float distance = sqrtf(delta.x * delta.x + delta.y * delta.y);

		// If in range, return it
		if (distance <= spellRange) return clickedTile;

		std::cout << "BrUh" << "\n";

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

	float isoRadius = game.registry.get<SpellStats>(*spell).range / 5;
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
			toggleDrawingModule(game, drawingModule);
		}

		drawingModuleDotsCheck(game, drawingModule);

		BeginDrawing();
		ClearBackground(DARKGREEN);

		// Draw isometric tiles
		//for (int y = 0; y < MAP_HEIGHT; ++y) {
		//	for (int x = 0; x < MAP_WIDTH; ++x) {
		//		DrawIsoTile(x, y, GRAY);
		//	}
		//}
		auto& camera = game.registry.get<CameraObject>(cameraEntity).camera;
		BeginMode2D(camera);
		drawVisibleTiles(game, camera);

		//std::cout << player.position.x << " | " << player.position.y << "\n";

		// Draw player cube
		Vector2 playerPosIso = isoToScreen(player.position.x, player.position.y);
		drawPill(playerPosIso, 16, 24, RED);
		updateCamera(game, playerPosIso, cameraEntity);
		drawSpellRangeCircle(game, player.position, spellCasted, BLACK);

		std::optional<Vector2> spellTarget = handleSpellTargeting(game, camera, spellCasted, drawingModule, player.position);
		if (spellTarget) {
			std::cout << (*spellTarget).x << " | " << (*spellTarget).y << "\n";
			//std::cout << player.position.x << " | " << player.position.y << "\n";
			DrawLineEx(playerPosIso, isoToScreen((*spellTarget).x, (*spellTarget).y), 5.0f, PURPLE);
		}
		EndMode2D();

		drawCastedSpell(game, spellCasted);
		renderDrawingModule(game, drawingModule);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
