#pragma once

#include <raylib.h>
#include <string>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include "../../components/spell.hpp"
#include "../../components/game_object.hpp"

using json = nlohmann::json;

DamageType parseDamageType(const std::string& str);
ShapeType parseShapeType(const std::string& str);
LevelType parseDamageType(const int level);

Color damageTypeToColor(DamageType type);

std::string shapeTypeToString(ShapeType type);
std::string damageTypeToString(DamageType type);
std::string levelTypeToString(LevelType level);

DamageType getDamageFromRotations(std::bitset<5> pattern);
LevelType getLevelFromRotations(std::bitset<5> pattern);

std::optional<entt::entity> findSpell(GameObjects& game, LevelType level, DamageType damageType);