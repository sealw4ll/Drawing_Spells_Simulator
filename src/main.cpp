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
	player.position = { MAP_WIDTH / 2, MAP_HEIGHT / 2 };
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
	Vector2 bottomRight = GetScreenToWorld2D({ game.screenWidth, game.screenHeight }, camera);

	// Convert the corners to isometric tile coordinates
	Vector2 isoTopLeft = screenToIso(topLeft.x, topLeft.y);
	Vector2 isoBottomRight = screenToIso(bottomRight.x, bottomRight.y);

	int minX = static_cast<int>(floor(isoTopLeft.x)) - 2;
	int maxX = static_cast<int>(ceil(isoBottomRight.x)) + 2;
	int minY = static_cast<int>(floor(isoTopLeft.y)) - 2;
	int maxY = static_cast<int>(ceil(isoBottomRight.y)) + 2;

	for (int y = minY; y <= maxY; ++y) {
		for (int x = minX; x <= maxX; ++x) {
			if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT) continue;
			drawIsoTile(x, y, GRAY);
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
	while (!WindowShouldClose()) {
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

		drawCastedSpell(game, spellCasted);
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

		// Draw player cube
		Vector2 playerPosIso = isoToScreenVector(player.position);
		drawPill(playerPosIso, 16, 24, RED);
		updateCamera(game, playerPosIso, cameraEntity);
		//DrawCube({ playerPos.x, playerPos.y - TILE_HEIGHT / 2.0f, 0 }, 20, 20, 20, RED);
		//DrawCubeWires({ playerPos.x, playerPos.y - TILE_HEIGHT / 2.0f, 0 }, 20, 20, 20, BLACK);
		EndMode2D();

		renderDrawingModule(game, drawingModule);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
