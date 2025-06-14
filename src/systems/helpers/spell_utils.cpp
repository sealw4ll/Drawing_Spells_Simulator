#include "spell_utils.hpp"

// DAMAGE HELPERS

// Function to parse the damage type from a string
DamageType parseDamageType(const std::string& str) {
	if (str == "fire") return DamageType::FIRE;
	if (str == "thunder") return DamageType::THUNDER;
	if (str == "ice") return DamageType::ICE;
	if (str == "lightning") return DamageType::LIGHTNING;
	if (str == "psychic") return DamageType::PSYCHIC;
	if (str == "force") return DamageType::FORCE;
	return DamageType::NONE;
}

// Function to convert a DamageType enum to a string
std::string damageTypeToString(DamageType type) {
	switch (type) {
	case DamageType::FIRE: return "fire damage";
	case DamageType::THUNDER: return "thunder damage";
	case DamageType::ICE: return "ice damage";
	case DamageType::LIGHTNING: return "lightning damage";
	case DamageType::PSYCHIC: return "psychic damage";
	case DamageType::FORCE: return "force damage";
	default: return "none";
	}
}

// Function to parse the level type from a int
LevelType parseDamageType(const int level) {
	if (level == 0) return LevelType::CANTRIP;
	if (level == 1) return LevelType::LEVEL1;
	if (level == 2) return LevelType::LEVEL2;
	if (level == 3) return LevelType::LEVEL3;
	if (level == 4) return LevelType::LEVEL4;
	if (level == 5) return LevelType::LEVEL5;
	return LevelType::NONE;
}

// Function to get color from damage type
Color damageTypeToColor(DamageType type) {
	switch (type) {
	case DamageType::FIRE: return RED;
	case DamageType::THUNDER: return YELLOW;
	case DamageType::ICE: return SKYBLUE;
	case DamageType::LIGHTNING: return DARKBLUE;
	case DamageType::PSYCHIC: return VIOLET;
	case DamageType::FORCE: return LIGHTGRAY;
	case DamageType::NONE: return BLACK;
	default: return BLACK;
	}
}

// SHAPE HELPERS

// Function to parse the shape type from a string
ShapeType parseShapeType(const std::string& str) {
	if (str == "target") return ShapeType::TARGET;
	if (str == "circle") return ShapeType::CIRCLE;
	if (str == "square") return ShapeType::SQUARE;
	if (str == "cone") return ShapeType::CONE;
	if (str == "line") return ShapeType::LINE;
	return ShapeType::NONE;
}

// Function to convert a ShapeType enum to a string
std::string shapeTypeToString(ShapeType type) {
	switch (type) {
	case ShapeType::TARGET: return "target";
	case ShapeType::CIRCLE: return "circle";
	case ShapeType::SQUARE: return "square";
	case ShapeType::CONE: return "cone";
	case ShapeType::LINE: return "line";
	default: return "none";
	}
}

// LEVEL HELPERS

std::string levelTypeToString(LevelType level) {
	switch (level) {
	case LevelType::CANTRIP: return "cantrip spell";
	case LevelType::LEVEL1: return "level 1 spell";
	case LevelType::LEVEL2: return "level 2 spell";
	case LevelType::LEVEL3: return "level 3 spell";
	case LevelType::LEVEL4: return "level 4 spell";
	case LevelType::LEVEL5: return "level 5 spell";
	default: return "Invalid spell";
	}
}

// ROTATION FUNCTIONS

DamageType getDamageFromRotations(std::bitset<5> pattern) {
	std::bitset<5> rotated = pattern;
	for (int i = 0; i < 5; ++i) {
		auto it = patternToDamageType.find(rotated);
		if (it != patternToDamageType.end())
			return it->second;

		// rotate left by 1
		bool msb = rotated[4];
		rotated <<= 1;
		rotated[0] = msb;
	}
	return DamageType::NONE;
}

LevelType getLevelFromRotations(std::bitset<5> pattern) {
	std::bitset<5> rotated = pattern;
	for (int i = 0; i < 5; ++i) {
		auto it = patternToLevelType.find(rotated);
		if (it != patternToLevelType.end()) {
			return it->second; // return the mapped LevelType
		}

		// rotate left by 1
		bool msb = rotated[4];
		rotated <<= 1;
		rotated[0] = msb;
	}
	return LevelType::NONE;
}


//find spell helper
std::optional<entt::entity> findSpell(GameObjects& game, LevelType level, DamageType damageType) {
	auto view = game.registry.view<SpellIdentifier>();
	for (auto [entity, id] : view.each()) {
		if (id.level == level && id.damageType == damageType) {
			return entity;
		}
	}

	return std::nullopt;
}