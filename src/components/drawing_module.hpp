#pragma once

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
	float moduleWidth = 300.0f;
	float moduleHeight = 300.0f;
	float dotRadius = 200.0f;
};