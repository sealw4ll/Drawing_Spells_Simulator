#include "spell_utils.hpp"

// Function to parse the damage type from a string
DamageType parseDamageType(const std::string& str) {
	if (str == "fire") return DamageType::FIRE;
	if (str == "thunder") return DamageType::THUNDER;
	if (str == "ice") return DamageType::ICE;
	if (str == "lightning") return DamageType::LIGHTNING;
	if (str == "psychic") return DamageType::PSYCHIC;
	return DamageType::NONE;
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