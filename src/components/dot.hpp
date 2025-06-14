#pragma once

#include <raylib.h>
#include <vector>

struct Dot {
	Vector2 position = {0, 0};
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