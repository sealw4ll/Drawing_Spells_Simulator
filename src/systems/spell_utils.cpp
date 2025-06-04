#include "spell_utils.hpp"

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

// Function to convert a DamageType enum to a string
std::string damageTypeToString(DamageType type) {
	switch (type) {
	case DamageType::FIRE: return "fire damage";
	case DamageType::THUNDER: return "thunder damage";
	case DamageType::ICE: return "ice damage";
	case DamageType::LIGHTNING: return "lightning damage";
	case DamageType::PSYCHIC: return "psychic damage";
	default: return "none";
	}
}

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

// utils for checking JSON data
bool isValidString(const json& j, const std::string& key) {
	return j.contains(key) && j[key].is_string();
}

bool isValidNumber(const json& j, const std::string& key) {
	return j.contains(key) && j[key].is_number();
}

bool isNonNegative(const json& j, const std::string& key) {
	return isValidNumber(j, key) && j[key].get<double>() >= 0;
}

bool isValidShape(const json& j) {
	if (!j.contains("shape") || !j["shape"].is_object()) return false;

	const auto& shape = j["shape"];

	if (!shape.contains("type") || !shape["type"].is_string()) return false;
	if (!shape.contains("size") || !isNonNegative(shape, "size")) return false;
	return true;
}