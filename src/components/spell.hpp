#pragma once

#include <string>
#include <bitset>
#include <unordered_map>

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
	double size;
};

struct SpellStats {
	double range;
	double damage;
	double duration;
	double castingTime;
};

struct SpellIdentifier {
	LevelType level;
	DamageType damageType;
};

struct Spell {
	std::string name;
};

const std::unordered_map<std::bitset<5>, DamageType> patternToDamageType = {
	{std::bitset<5>("00001"), DamageType::FIRE},
	{std::bitset<5>("00010"), DamageType::THUNDER},
	{std::bitset<5>("00101"), DamageType::ICE},
	{std::bitset<5>("00110"), DamageType::LIGHTNING},
	{std::bitset<5>("00111"), DamageType::PSYCHIC},
	{std::bitset<5>("01001"), DamageType::FORCE},
};

const std::unordered_map<std::bitset<5>, LevelType> patternToLevelType = {
	{std::bitset<5>("00001"), LevelType::CANTRIP},
	{std::bitset<5>("00010"), LevelType::LEVEL1},
	{std::bitset<5>("00101"), LevelType::LEVEL2},
	{std::bitset<5>("00110"), LevelType::LEVEL3},
	{std::bitset<5>("00111"), LevelType::LEVEL4},
	{std::bitset<5>("01001"), LevelType::LEVEL5},
};