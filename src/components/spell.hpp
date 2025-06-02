#pragma once

#include <string>

enum class DamageType
{
	NONE,
	FIRE,
	THUNDER,
	ICE,
	LIGHTNING,
	PSYCHIC,
};
enum class ShapeType
{
	NONE,
	TARGET,
	CIRCLE,
	SQUARE,
	CONE,
	LINE,
};

struct SpellShape {
	ShapeType type;
	double size;
};

struct SpellStats {
	double range;
	double damage;
	double duration;
	double castingTime;
};

struct SpellIdentifier {
	int level;
	DamageType damageType;
};

struct Spell {
	std::string name;
};
