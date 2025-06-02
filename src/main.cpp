#include "main.hpp"

using namespace std;

struct Dot {
	Vector2 position;
	bool isHovered = false;
};

bool IsMouseOverDot(Vector2 mouse, Vector2 dotPos, double radius = 20.0f) {
	return Vector2Distance(mouse, dotPos) <= radius;
}

int main() {
	SetConfigFlags(FLAG_FULLSCREEN_MODE);

	InitWindow(0, 0, "Drawing Spells Simulator");
	SetTargetFPS(60);

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	entt::registry registry;
	loadSpells(registry, "../spells/");

	const int numDots = 5;
	std::vector<Dot> dots;
	std::vector<std::pair<int, int>> connections;

	float radius = 200.0f;
	const Vector2 center = { (static_cast<float>(screenWidth) / 2.0f), (static_cast<float>(screenHeight) / 2.0f )};

	std::cout << "x: " << center.x << " | y: " << center.y << "\n";

	float angleOffset = -PI / 2;
	// Place 5 dots in a circle
	for (int i = 0; i < numDots; ++i) {
		float angle = angleOffset + i * (2 * PI / numDots);
		Vector2 pos = {
			center.x + radius * cosf(angle),
			center.y + radius * sinf(angle)
		};
		dots.push_back({ pos });
	}

	int selectedDot = -1;

	while (!WindowShouldClose()) {
		Vector2 mouse = GetMousePosition();
		
		// Update hover state
		for (int i = 0; i < numDots; ++i) {
			dots[i].isHovered = IsMouseOverDot(mouse, dots[i].position);
		}

		// Mouse press
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			for (int i = 0; i < numDots; ++i) {
				if (dots[i].isHovered) {
					selectedDot = i;
					break;
				}
			}
		}

		// Mouse release
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && selectedDot != -1) {
			for (int i = 0; i < numDots; ++i) {
				if (i != selectedDot && dots[i].isHovered) {
					bool alreadyConnected = false;
					for (auto& c : connections) {
						if ((c.first == selectedDot && c.second == i) ||
							(c.first == i && c.second == selectedDot)) {
							alreadyConnected = true;
							break;
						}
					}
					if (!alreadyConnected)
						connections.emplace_back(selectedDot, i);
					break;
				}
			}
			selectedDot = -1;
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);

		// Draw connections
		for (auto& c : connections) {
			DrawLineEx(dots[c.first].position, dots[c.second].position, 5.0f, DARKGRAY);
		}

		// Draw dragging line
		if (selectedDot != -1) {
			DrawLineEx(dots[selectedDot].position, mouse, 5.0f, LIGHTGRAY);
		}

		// Draw dots
		for (auto& d : dots) {
			Color color = d.isHovered ? RED : BLACK;
			DrawCircleV(d.position, 15, color);
		}

		DrawText("Click and drag to connect two dots", 10, 10, 20, GRAY);

		EndDrawing();
	}
	//auto view = registry.view<Spell, SpellIdentifier, SpellStats, SpellShape>();

	/*view.each([](auto entity, auto& spell, auto& identifier, auto& stats, auto& shape) {
		std::cout << "Spell name: " << spell.name << "\n";
		std::cout << "Spell Level: " << identifier.level << "\n";
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
