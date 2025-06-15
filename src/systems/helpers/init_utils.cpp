#include "init_utils.hpp"

void initGameObjects(GameObjects& game) {
	/*game.screenWidth = static_cast<float>(GetScreenWidth());
	game.screenHeight = static_cast<float>(GetScreenHeight());*/
	game.centerScreen = Vector2{ game.screenWidth / 2.0f, game.screenHeight / 2.0f };
}

entt::entity initPlayer(GameObjects& game) {
	auto entity = game.registry.create();
	Player player;
	player.position = { MAP_WIDTH / 2.0f, MAP_HEIGHT / 2.0f };
	game.registry.emplace<Player>(entity, std::move(player));
	return entity;
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