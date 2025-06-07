#include "spell_parser.hpp"

// Function to check if spell parsing has errors and print them
bool printSpellParsingErrors(const std::string path, std::optional<Spell>& spell, std::optional<SpellIdentifier>& id, std::optional<SpellStats>& stats, std::optional<SpellShape>& shape) {
	std::vector<std::string> errors;

	if (!spell) errors.emplace_back("parse spell name fail");
	if (!id)    errors.emplace_back("parse spell identifier fail");
	if (!stats) errors.emplace_back("parse spell stats fail");
	if (!shape) errors.emplace_back("parse spell shape fail");

	if (!errors.empty()) {
		std::cerr << "Error(s) in file: " << path << "\n";
		for (const auto& error : errors) {
			std::cerr << "  - " << error << "\n";
		}
		return true;
	}
	return false;
}

// Function to parse the spell data from JSON
std::optional<Spell> parseSpell(const json& j) {
	if (!isValidString(j, "name")) return std::nullopt;

	return Spell{ j["name"].get<std::string>() };
}

// Function to parse the spell identifier from JSON
std::optional<SpellIdentifier> parseIdentifier(const json& j) {
	if (!j.contains("level") || !j.contains("damage_type")) return std::nullopt;

	int levelInt = j["level"].get<int>();
	LevelType level = parseDamageType(levelInt);
	std::string typeStr = j["damage_type"].get<std::string>();
	DamageType type = parseDamageType(typeStr);

	if (level == LevelType::NONE || type == DamageType::NONE) return std::nullopt;
	return SpellIdentifier{ level, type };
}

// Function to parse the spell stats from JSON
std::optional<SpellStats> parseStats(const json& j) {
	if (!isNonNegative(j, "range") || !isValidNumber(j, "damage") ||
		!isNonNegative(j, "duration") || !isNonNegative(j, "casting_time")) {
		return std::nullopt;
	}

	return SpellStats{
		j["range"].get<double>(),
		j["damage"].get<double>(),
		j["duration"].get<double>(),
		j["casting_time"].get<double>()
	};
}

// Function to parse the spell shape from JSON
std::optional<SpellShape> parseShape(const json& j) {
	if (!isValidShape(j)) return std::nullopt;

	auto shape = j["shape"];

	ShapeType type = parseShapeType(shape["type"].get<std::string>());
	if (type == ShapeType::NONE) return std::nullopt;
	return SpellShape{
		type,
		shape["size"].get<double>()
	};
}