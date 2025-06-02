#include "main.hpp"

using namespace std;

int main() {
	InitWindow(800, 600, "Spell System Test");
	SetTargetFPS(60);

	entt::registry registry;
	loadSpells(registry, "../spells/");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);

		// Just debug draw spell names
		int y = 20;
		registry.view<Spell>().each([&](auto entity, auto& name) {
			DrawText(name.name.c_str(), 20, y, 20, BLACK);
			y += 30;
			});

		EndDrawing();
	}
	auto view = registry.view<Spell, SpellIdentifier, SpellStats, SpellShape>();

	view.each([](auto entity, auto& spell, auto& identifier, auto& stats, auto& shape) {
		std::cout << "Spell name: " << spell.name << "\n";
		std::cout << "Spell Level: " << identifier.level << "\n";
		std::cout << "Spell damage type: " << damageTypeToString(identifier.damageType) << "\n";
		std::cout << "Spell range: " << stats.range << "\n";
		std::cout << "Spell damage: " << stats.damage << "\n";
		std::cout << "Spell duration: " << stats.duration << "\n";
		std::cout << "Spell casting time: " << stats.castingTime << "\n";
		std::cout << "Spell shape type: " << shapeTypeToString(shape.type) << "\n";
		std::cout << "Spell shape size: " << shape.size << "\n\n\n";
		});

	CloseWindow();
	return 0;
}
