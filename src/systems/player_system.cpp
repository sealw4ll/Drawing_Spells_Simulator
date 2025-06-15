#include "player_system.hpp"

void playerMovement(Player& player) {
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
}