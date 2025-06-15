#include "enemy_system.hpp"

static float getRandomFloat(float min, float max) {
	return min + (GetRandomValue(0, 10000) / 10000.0f) * (max - min);
}

void enemySpawnSystem(GameObjects& game, float spawnInterval) {
	static float lastSpawnTime = 0;
	float currentTime = GetTime();
	float screenWidth = game.screenWidth;
	float screenHeight = game.screenHeight;
	int numEnemies = game.registry.view<Enemy>().size();
	if (numEnemies >= 5) return;


	if (currentTime - lastSpawnTime >= spawnInterval || numEnemies == 0) {
		auto playerEntity = game.registry.view<Player>().front();
		Vector2 playerPos = game.registry.get<Player>(playerEntity).position;

		float minX = playerPos.x - (screenWidth / TILE_WIDTH * 0.8);
		float minY = playerPos.y - (screenHeight / TILE_HEIGHT * 0.8);
		float maxX = playerPos.x + (screenWidth / TILE_WIDTH * 0.8);
		float maxY = playerPos.y + (screenHeight / TILE_HEIGHT * 0.8);
		// Create enemy entity
		entt::entity enemy = game.registry.create();

		Vector2 pos = { getRandomFloat(minX, maxX), getRandomFloat(minY, maxY) };
		float hp = GetRandomValue(20, 50);
		// Assign components
		game.registry.emplace<Enemy>(enemy, pos, hp);
		game.registry.emplace<EnemyActiveComponent>(enemy, hp);

		lastSpawnTime = currentTime;
	}
}

void drawHpBar(Vector2 position, float hp, float maxHp, float width, float height, Color fill, Color outline) {
	// Position the HP bar slightly above the enemy
	float offsetY = height / 2 + 15; // Adjust as needed

	Vector2 barPos = { position.x - width / 2, position.y - offsetY };

	// Compute HP percentage
	float hpPercentage = hp / maxHp;
	float hpWidth = width * hpPercentage;

	// Draw outline
	DrawRectangle(barPos.x - 1, barPos.y - 1, width + 2, height / 2, outline);

	// Draw HP fill
	DrawRectangle(barPos.x, barPos.y, hpWidth, height / 2 - 2, fill);
}

void renderEnemies(GameObjects& game) {
	auto view = game.registry.view<Enemy, EnemyActiveComponent>();
	for (auto [entity, enemy, enemyActive] : view.each()) {
		auto& enemy = game.registry.get<Enemy>(entity);
		auto& enemyActive = game.registry.get<EnemyActiveComponent>(entity);

		drawPill(isoToScreen(enemy.position.x, enemy.position.y), 16, 24, RED, BLACK);
		drawHpBar(isoToScreen(enemy.position.x, enemy.position.y), enemyActive.currentHp, enemy.hp, 40, 16, RED, WHITE);
	}
}