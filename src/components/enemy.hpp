#pragma once

#include <raylib.h>

struct Enemy {
	Vector2 position;
	float hp = 50;
};

struct EnemyActiveComponent {
	float currentHp;
};