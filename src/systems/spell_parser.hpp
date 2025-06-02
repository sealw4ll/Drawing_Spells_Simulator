#pragma once

#include <optional>
#include <nlohmann/json.hpp>
#include <iostream>

#include "spell_utils.hpp"

using json = nlohmann::json;

bool printSpellParsingErrors(const std::string path, std::optional<Spell>& spell, std::optional<SpellIdentifier>& id, std::optional<SpellStats>& stats, std::optional<SpellShape>& shape);

std::optional<Spell> parseSpell(const nlohmann::json& j);
std::optional<SpellIdentifier> parseIdentifier(const nlohmann::json& j);
std::optional<SpellStats> parseStats(const nlohmann::json& j);
std::optional<SpellShape> parseShape(const nlohmann::json& j);
