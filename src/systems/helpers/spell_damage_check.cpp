#include "spell_damage_check.hpp"
#include <iostream>

std::tuple<Vector2, Vector2, Vector2> getIsoTrianglePoints(Vector2 start, Vector2 end, float size) {
	Vector2 screenStart = isoToScreen(start.x, start.y);
	Vector2 dir = { end.x - start.x, end.y - start.y };
	float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
	if (length == 0) return {start, start, start};

	dir.x /= length;
	dir.y /= length;

	Vector2 isoTip = { 
		start.x + dir.x * size,
		start.y + dir.y * size
	};

	Vector2 screenTip = isoToScreen(isoTip.x, isoTip.y);
	Vector2 perp = { -dir.y, dir.x };
	float halfWidth = size / 2.0f;

	Vector2 isoV2 = {
		isoTip.x + perp.x * halfWidth,
		isoTip.y + perp.y * halfWidth
	};

	Vector2 isoV3 = {
		isoTip.x - perp.x * halfWidth,
		isoTip.y - perp.y * halfWidth
	};

	Vector2 screenV2 = isoToScreen(isoV2.x, isoV2.y);
	Vector2 screenV3 = isoToScreen(isoV3.x, isoV3.y);
	return { screenStart, screenV2, screenV3 };
}

bool isPointInCircle(Vector2 point, Vector2 center, float radius) {
	float dx = point.x - center.x;
	float dy = point.y - center.y;
	return (dx * dx + dy * dy) <= radius * radius;
}

bool isPointInRectangle(Vector2 point, Vector2 center, float width, float height) {
	return fabs(point.x - center.x) <= width / 2 &&
		fabs(point.y - center.y) <= height / 2;
}

bool isPointNearLine(Vector2 point, Vector2 a, Vector2 b, float thickness) {
	// Vector math: closest point on line segment
	Vector2 ab = { b.x - a.x, b.y - a.y };
	Vector2 ap = { point.x - a.x, point.y - a.y };
	float abLenSq = ab.x * ab.x + ab.y * ab.y;
	float t = fmaxf(0, fminf(1, (ap.x * ab.x + ap.y * ab.y) / abLenSq));
	Vector2 closest = { a.x + ab.x * t, a.y + ab.y * t };

	float dx = point.x - closest.x;
	float dy = point.y - closest.y;
	return (dx * dx + dy * dy) <= (thickness * thickness);
}

bool isPointInTriangle(Vector2 point, Vector2 start, Vector2 end, float size) {
	std::tuple<Vector2, Vector2, Vector2> trianglePoints = getIsoTrianglePoints(start, end, size);

	auto [a, b, c] = trianglePoints; // Already in screen space

	point = isoToScreen(point.x, point.y);
	// Compute the area of the triangle using cross product
	float area = fabs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y));

	// Compute sub-triangle areas
	float area1 = fabs((a.x - point.x) * (b.y - point.y) - (b.x - point.x) * (a.y - point.y));
	float area2 = fabs((b.x - point.x) * (c.y - point.y) - (c.x - point.x) * (b.y - point.y));
	float area3 = fabs((c.x - point.x) * (a.y - point.y) - (a.x - point.x) * (c.y - point.y));

	// If sum of sub-areas equals the full triangle area, the point is inside
	return fabs(area - (area1 + area2 + area3)) < 0.01f;
}