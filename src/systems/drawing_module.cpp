#include "drawing_module.hpp"

static bool isMouseOverDot(Vector2 mouse, Vector2 dotPos, double radius = 20.0f) {
	return Vector2Distance(mouse, dotPos) <= radius;
}

// handle mouse check for drawing module
void drawingModuleDotsCheck(GameObjects& game, entt::entity& drawingModule) {
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

// Render the drawing module
void renderDrawingModule(GameObjects& game, entt::entity& drawingModule) {
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

	//DrawText("Click and drag to connect two dots", 10, 10, 20, GRAY);
}


// Initialize drawing module
entt::entity createDrawingModule(GameObjects& game) {
	DrawingModule module;

	module.selectedDot = -1;
	module.numDots = 5;
	module.isVisible = false;

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

std::optional<entt::entity> printAndResetConnections(GameObjects& game, entt::entity& drawingModule) {
	auto& module = game.registry.get<DrawingModule>(drawingModule);
	entt::entity spellEntity;
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
	module.connections.clear();
	return findSpell(game, getLevelFromRotations(level), getDamageFromRotations(damageType));
}