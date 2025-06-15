#include "main.hpp"

int main() {
	GameObjects game;

	// init game object and window
	//SetConfigFlags(FLAG_FULLSCREEN_MODE);
	initGameObjects(game);
	InitWindow(game.screenWidth, game.screenHeight, "Drawing Spells Simulator");
	SetTargetFPS(60);

	// load spells
	loadSpells(game.registry, "../spells/");

	// init player
	entt::entity playerEntity = initPlayer(game);
	Player& player = game.registry.get<Player>(playerEntity);
	entt::entity cameraEntity = initCamera(game, player);
	entt::entity drawingModule = createDrawingModule(game);

	// declare spells
	std::optional<entt::entity> spellCasted = std::nullopt;
	std::optional<entt::entity> activeSpell = std::nullopt;
	std::optional<Vector2> spellTarget = std::nullopt;

	while (!WindowShouldClose()) {
		//std::this_thread::sleep_for(std::chrono::milliseconds(200));
		playerMovement(player);
		game.mouse = GetMousePosition();

		// hold to open drawing module
		if (IsKeyPressed(KEY_LEFT_CONTROL)) {
			toggleDrawingModule(game, drawingModule);
		}

		// Release to close drawing module and cast spell
		if (IsKeyReleased(KEY_LEFT_CONTROL)) {
			spellCasted = printAndResetConnections(game, drawingModule);
			toggleDrawingModule(game, drawingModule);
			spellTarget.reset();
		}

		// drawing dots handler
		drawingModuleDotsCheck(game, drawingModule);

		BeginDrawing();
		ClearBackground(DARKGREEN);

		// camera handling
		auto& camera = game.registry.get<CameraObject>(cameraEntity).camera;
		BeginMode2D(camera);
		
		// draw background 
		drawVisibleTiles(game, camera);

		// Draw player cube
		Vector2 playerPosIso = isoToScreen(player.position.x, player.position.y);
		updateCamera(game, playerPosIso, cameraEntity);
		drawSpellRangeCircle(game, player.position, spellCasted, BLACK);

		// handle spell casting
		std::optional<Vector2> spellTarget = handleSpellTargeting(game, camera, spellCasted, drawingModule, player.position);
		if (spellTarget) activeSpellHandler(game, activeSpell, spellTarget, spellCasted, player.position);

		// draw spells and player
		drawSpells(game, playerPosIso);
		drawPill(playerPosIso, 16, 24, ORANGE, BLACK);

		// draw enemies
		enemySpawnSystem(game);
		renderEnemies(game);

		EndMode2D();

		// write spell words
		writeCastedSpell(game, spellCasted);

		//render drawing module
		renderDrawingModule(game, drawingModule);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
