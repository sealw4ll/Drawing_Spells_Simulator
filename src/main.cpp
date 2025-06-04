#include "main.hpp"

using namespace std;

struct Dot {
	Vector2 position;
	bool isHovered = false;
};

struct DrawingModule {
	std::vector<Dot> dots;
	std::set<std::pair<int, int>> connections;
	int selectedDot = -1;
	int numDots = 5;
	bool isVisible = false;
	Vector2 Position = { 0, 0 };
	float moduleWidth = 300;
	float moduleHeight = 300;
	float dotRadius = 200.0f;
};

struct GameObjects {
	Vector2 mouse = {0, 0};
	float screenWidth = 800;
	float screenHeight = 600;
	Vector2 centerScreen = {400, 300};
	entt::registry registry;
};

DamageType getDamageFromRotations(std::bitset<5> pattern) {
	std::bitset<5> rotated = pattern;
	for (int i = 0; i < 5; ++i) {
		auto it = patternToDamageType.find(rotated);
		if (it != patternToDamageType.end())
			return it->second;

		// rotate left by 1
		bool msb = rotated[4];
		rotated <<= 1;
		rotated[0] = msb;
	}
	return DamageType::NONE;
}

LevelType getLevelFromRotations(std::bitset<5> pattern) {
	std::bitset<5> rotated = pattern;
	for (int i = 0; i < 5; ++i) {
		auto it = patternToLevelType.find(rotated);
		if (it != patternToLevelType.end())
			return it->second;

		// rotate left by 1
		bool msb = rotated[4];
		rotated <<= 1;
		rotated[0] = msb;
	}
	return LevelType::NONE;
}

bool isMouseOverDot(Vector2 mouse, Vector2 dotPos, double radius = 20.0f) {
	return Vector2Distance(mouse, dotPos) <= radius;
}

void drawingModuleDotsCheck(GameObjects& game, entt::entity& drawingModule){
	// Update hover state
	auto& module = game.registry.get<DrawingModule>(drawingModule);

	if (module.isVisible == false) return;

	// Update hover state
	for (int i = 0; i < module.numDots; ++i) {
		module.dots[i].isHovered = isMouseOverDot(game.mouse, module.dots[i].position);
	}

	// Mouse press
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		for (int i = 0; i < module.numDots; ++i) {
			if (module.dots[i].isHovered) {
				module.selectedDot = i;
				break;
			}
		}
	}

	// Mouse release
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && module.selectedDot != -1) {
		for (int i = 0; i < module.numDots; ++i) {
			if (i != module.selectedDot && module.dots[i].isHovered) {
				int a = std::min(module.selectedDot, i);
				int b = std::max(module.selectedDot, i);
				module.connections.insert({ a, b });
				module.selectedDot = -1;
				break;
			}
		}
		module.selectedDot = -1;
	}
}

void renderDrawingModule(GameObjects& game, entt::entity& drawingModule){
	auto& module = game.registry.get<DrawingModule>(drawingModule);

	if (module.isVisible == false) return;
	// Draw connections
	for (auto& c : module.connections) {
		DrawLineEx(module.dots[c.first].position, module.dots[c.second].position, 5.0f, DARKGRAY);
	}

	// Draw dragging line
	if (module.selectedDot != -1) {
		DrawLineEx(module.dots[module.selectedDot].position, game.mouse, 5.0f, LIGHTGRAY);
	}

	// Draw dots
	for (auto& d : module.dots) {
		Color color = d.isHovered ? RED : BLACK;
		DrawCircleV(d.position, 15, color);
	}

	DrawText("Click and drag to connect two dots", 10, 10, 20, GRAY);
}

entt::entity createDrawingModule(GameObjects& game) {
	DrawingModule module;

	module.dotRadius = std::min(game.screenWidth, game.screenHeight) * 0.25f;
	float angleOffset = -PI / 2;
	for (int i = 0; i < module.numDots; ++i) {
		float angle = angleOffset + i * (2 * PI / module.numDots);
		float x = game.centerScreen.x + cos(angle) * module.dotRadius;
		float y = game.centerScreen.y + sin(angle) * module.dotRadius;
		module.dots.push_back({ Vector2{ x, y }, false });
	}

	auto entity = game.registry.create();
	game.registry.emplace<DrawingModule>(entity, std::move(module));
	return entity;
}

void toggleDrawingModule(GameObjects& game, entt::entity& drawingModule) {
	auto& module = game.registry.get<DrawingModule>(drawingModule);
	module.isVisible = !module.isVisible;
}

std::optional<entt::entity> findSpell(GameObjects& game, LevelType level, DamageType damageType) {
	auto view = game.registry.view<SpellIdentifier>();
	for (auto [entity, id] : view.each()) {
		//const auto& id = game.registry.get<SpellIdentifier>(entity);
		if (id.level == level && id.damageType == damageType) {
			return entity;
		}
	}

	return std::nullopt;
}

void printAndResetConnections(GameObjects& game, entt::entity& drawingModule) {
	auto& module = game.registry.get<DrawingModule>(drawingModule);

	auto numDots = module.numDots;
	std::bitset<5> level = 0;
	std::bitset<5> damageType = 0;

	for (int i = 0; i < numDots; ++i) {
		int next = (i + 1) % numDots;
		int nextNext = (i + 2) % numDots;

		// Check if there's a connection from i to next or nextNext
		bool hasNext = module.connections.count({ std::min(i, next), std::max(i, next) }) > 0;
		bool hasNextNext = module.connections.count({ std::min(i, nextNext), std::max(i, nextNext) }) > 0;

		if (hasNext)
			level |= (1 << (numDots - 1 - i)); // Left-shifting from left-most bit

		if (hasNextNext)
			damageType |= (1 << (numDots - 1 - i));
	}
	// Clear connections
	/*std::bitset<5> levelBits(level);
	std::bitset<5> typeBits(type);*/
	std::cout << "Connections:\n" << "level: " << level << "\ndamage type: " << damageType << "\n";
	auto spellEntity = findSpell(game, getLevelFromRotations(level), getDamageFromRotations(damageType));
	if (!spellEntity){
		cout << "spell not found" << "\n";
	} else
	{
		auto& spellInfo = game.registry.get<Spell>(*spellEntity);
		cout << "I CAST: " << spellInfo.name << "\n";
	}

	module.connections.clear();
	cout << "count after clear: "<< module.connections.size() << "\n";
}

int main() {
	GameObjects game;

	//SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(800, 600, "Drawing Spells Simulator");
	SetTargetFPS(60);

	game.screenWidth = static_cast<float>(GetScreenWidth());
	game.screenHeight = static_cast<float>(GetScreenHeight());
	game.centerScreen = Vector2{ game.screenWidth / 2.0f, game.screenHeight / 2.0f };

	loadSpells(game.registry, "../spells/");

	entt::entity drawingModule = createDrawingModule(game);
	while (!WindowShouldClose()) {
		game.mouse = GetMousePosition();
		
		if (IsKeyPressed(KEY_M)) { // Toggle on 'M'
			cout << "opened/closed drawing" << "\n";
			toggleDrawingModule(game, drawingModule);
		}
		if (IsKeyPressed(KEY_P)) { // Print & reset on 'P'
			printAndResetConnections(game, drawingModule);
		}
		drawingModuleDotsCheck(game, drawingModule);

		BeginDrawing();
		ClearBackground(RAYWHITE);

		renderDrawingModule(game, drawingModule);
		EndDrawing();
	}
	//auto view = registry.view<Spell, SpellIdentifier, SpellStats, SpellShape>();

	/*view.each([](auto entity, auto& spell, auto& identifier, auto& stats, auto& shape) {
		std::cout << "Spell name: " << spell.name << "\n";
		std::cout << "Spell Level: " << levelTypeToString(identifier.level) << "\n";
		std::cout << "Spell damage type: " << damageTypeToString(identifier.damageType) << "\n";
		std::cout << "Spell range: " << stats.range << "\n";
		std::cout << "Spell damage: " << stats.damage << "\n";
		std::cout << "Spell duration: " << stats.duration << "\n";
		std::cout << "Spell casting time: " << stats.castingTime << "\n";
		std::cout << "Spell shape type: " << shapeTypeToString(shape.type) << "\n";
		std::cout << "Spell shape size: " << shape.size << "\n\n\n";
		});*/

	CloseWindow();
	return 0;
}
