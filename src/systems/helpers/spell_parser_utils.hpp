#pragma once

#include <nlohmann/json.hpp>
#include "../../components/spell.hpp"

using json = nlohmann::json;

bool isValidString(const json& j, const std::string& key);
bool isValidNumber(const json& j, const std::string& key);
bool isNonNegative(const json& j, const std::string& key);
bool isValidShape(const json& j);