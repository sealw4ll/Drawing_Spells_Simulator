#include "spell_parser_utils.hpp"

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