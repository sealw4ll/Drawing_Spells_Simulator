#include "camera_system.hpp"

static void lerpCamera(Camera2D& camera, Vector2 targetPosition, float lerpAmount = 0.1f) {
	// Smoothly interpolate the camera target toward the targetPosition
	camera.target.x += (targetPosition.x - camera.target.x) * lerpAmount;
	camera.target.y += (targetPosition.y - camera.target.y) * lerpAmount;
}

void updateCamera(GameObjects& game, Vector2 playerPosition, entt::entity cameraEntity) {
	
	auto& cam = game.registry.get<CameraObject>(cameraEntity).camera;

	// Example: make camera follow the player
	cam.target = playerPosition;

	// Optionally lerp to new position
	Vector2 desiredPosition = { playerPosition.x + 0.0f, playerPosition.y + 200.0f};
	cam.target = Vector2Lerp(cam.target, desiredPosition, 0.1f);

	// Update raylib's camera
	//UpdateCamera(&cam, CAMERA_CUSTOM);
	lerpCamera(cam, playerPosition);
}