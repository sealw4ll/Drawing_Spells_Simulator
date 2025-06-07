#pragma once

#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../components/spell.hpp"
#include "helpers/spell_parser.hpp"
#include "helpers/spell_utils.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

// Function to load spells from a JSON file into the registry
void loadSpells(entt::registry& registry, const std::string& path);