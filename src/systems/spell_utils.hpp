#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "../components/spell.hpp"

using json = nlohmann::json;

DamageType parseDamageType(const std::string& str);
ShapeType parseShapeType(const std::string& str);
LevelType parseDamageType(const int level);

std::string shapeTypeToString(ShapeType type);
std::string damageTypeToString(DamageType type);
std::string levelTypeToString(LevelType level);

bool isValidString(const json& j, const std::string& key);
bool isValidNumber(const json& j, const std::string& key);
bool isNonNegative(const json& j, const std::string& key);
bool isValidShape(const json & j);
