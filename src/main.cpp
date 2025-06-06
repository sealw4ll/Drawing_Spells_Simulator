#include "main.hpp"

// Tile size
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 32;

// Grid size
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 10;

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
	player.position = { 5, 5 };
	game.registry.emplace<Player>(entity, std::move(player));
	return entity;
}

Vector2 IsoToScreenVector(Vector2 pos) {
	return {
		(pos.x - pos.y) * TILE_WIDTH / 2 + 400,
		(pos.x + pos.y) * TILE_HEIGHT / 2 + 100
	};
}

Vector2 IsoToScreenInt(int x, int y) {
	return {
		static_cast<float>((x - y) * TILE_WIDTH / 2 + 400),
		static_cast<float>((x + y) * TILE_HEIGHT / 2 + 100)
	};
}

int main() {
	GameObjects game;

	//SetConfigFlags(FLAG_FULLSCREEN_MODE);
	initGameObjects(game);
	entt::entity playerEntity = initPlayer(game);
	InitWindow(game.screenWidth, game.screenHeight, "Drawing Spells Simulator");
	SetTargetFPS(60);

	loadSpells(game.registry, "../spells/");

	entt::entity drawingModule = createDrawingModule(game);
	std::optional<entt::entity> spellCasted = std::nullopt;
	Player player = game.registry.get<Player>(playerEntity);
	while (!WindowShouldClose()) {
		// --- Input ---
		if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) player.position.y--;
		if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) player.position.x--;
		if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) player.position.y++;
		if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) player.position.x++;
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
		for (int y = 0; y < MAP_HEIGHT; ++y) {
			for (int x = 0; x < MAP_WIDTH; ++x) {
				Vector2 pos = IsoToScreenInt(x, y);
				DrawPolyLines({ pos.x, pos.y }, 4, TILE_WIDTH / 2.0f, 45.0f, GRAY);
			}
		}

		std::cout << "x: " << player.position.x << " | y: " << player.position.y << "\n";
		// Draw player cube
		Vector2 playerPos = IsoToScreenVector(player.position);
		DrawCube({ playerPos.x, playerPos.y - TILE_HEIGHT / 2.0f, 0 }, 20, 20, 20, RED);
		DrawCubeWires({ playerPos.x, playerPos.y - TILE_HEIGHT / 2.0f, 0 }, 20, 20, 20, BLACK);

		renderDrawingModule(game, drawingModule);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
