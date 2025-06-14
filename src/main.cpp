#include "main.hpp"

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

//Vector2 isoToScreenVector(Vector2 pos) {
//	return {
//		(pos.x - pos.y) * TILE_WIDTH / 2 + 400,
//		(pos.x + pos.y) * TILE_HEIGHT / 2 + 100
//	};
//}

//Vector2 isoToScreenInt(int x, int y) {
//	return {
//		static_cast<float>((x - y) * TILE_WIDTH / 2 + 400),
//		static_cast<float>((x + y) * TILE_HEIGHT / 2 + 100)
//	};
//}

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
		//drawPill(playerPosIso, 16, 24, RED, BLACK);
		drawSpellRangeCircle(game, player.position, spellCasted, BLACK);

		std::optional<Vector2> spellTarget = handleSpellTargeting(game, camera, spellCasted, drawingModule, player.position);
		if (spellTarget) {
			if (activeSpell) {
				game.registry.remove<SpellActiveComponent>(*activeSpell);
				activeSpell.reset();
			}

			 //Create new spell entity
			/*entt::entity spellEntity = game.registry.create();
			game.registry.emplace<Spell>(spellEntity, game.registry.get<Spell>(*spellCasted));
			game.registry.emplace<SpellStats>(spellEntity, game.registry.get<SpellStats>(*spellCasted));
			game.registry.emplace<SpellShape>(spellEntity, game.registry.get<SpellShape>(*spellCasted));
			game.registry.emplace<SpellIdentifier>(spellEntity, game.registry.get<SpellIdentifier>(*spellCasted));*/

			SpellActiveComponent module;
			module.position = *spellTarget;
			module.positionCasted = player.position;
			module.lifeTime = game.registry.get<SpellStats>(*spellCasted).duration;
			game.registry.emplace<SpellActiveComponent>(*spellCasted, std::move(module));

			activeSpell = *spellCasted;
			spellTarget = std::nullopt;
			spellCasted = std::nullopt;
		}
		//drawVerticalIsometricPill(player.position, 0.5, 1, RED, BLACK);
		drawSpells(game, playerPosIso);
		drawPill(playerPosIso, 16, 24, RED, BLACK);
		EndMode2D();
		writeCastedSpell(game, spellCasted);
		renderDrawingModule(game, drawingModule);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
