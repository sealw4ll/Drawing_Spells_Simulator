#pragma once

#include <string>
#include <bitset>
#include <unordered_map>
#include <raylib.h>

enum class DamageType {
	NONE,
	FIRE,
	THUNDER,
	ICE,
	LIGHTNING,
	PSYCHIC,
	FORCE,
};

enum class ShapeType {
	NONE,
	TARGET,
	CIRCLE,
	SQUARE,
	CONE,
	LINE,
};

enum class LevelType {
	CANTRIP,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	LEVEL5,
	NONE,
};

struct SpellShape {
	ShapeType type;
	float size;
};

struct SpellStats {
	float range;
	float damage;
	float duration;
	float castingTime;
};

struct SpellIdentifier {
	LevelType level;
	DamageType damageType;
};

struct Spell {
	std::string name;
	Vector2 position {0, 0};
};

struct SpellActiveComponent {
	float lifeTime;
	Vector2 position{ 0, 0 };
	Vector2 positionCasted{ 0, 0 };
};

const std::unordered_map<std::bitset<5>, DamageType> patternToDamageType = {
	{std::bitset<5>("00001"), DamageType::FIRE},
	{std::bitset<5>("00011"), DamageType::THUNDER},
	{std::bitset<5>("00101"), DamageType::ICE},
	{std::bitset<5>("00111"), DamageType::LIGHTNING},
	{std::bitset<5>("01101"), DamageType::PSYCHIC},
	{std::bitset<5>("11111"), DamageType::FORCE},
	{std::bitset<5>("01111"), DamageType::NONE},
};

const std::unordered_map<std::bitset<5>, LevelType> patternToLevelType = {
	{std::bitset<5>("00001"), LevelType::CANTRIP},
	{std::bitset<5>("00011"), LevelType::LEVEL1},
	{std::bitset<5>("00101"), LevelType::LEVEL2},
	{std::bitset<5>("00111"), LevelType::LEVEL3},
	{std::bitset<5>("01011"), LevelType::LEVEL4},
	{std::bitset<5>("11111"), LevelType::LEVEL5},
	{std::bitset<5>("01111"), LevelType::NONE},
};