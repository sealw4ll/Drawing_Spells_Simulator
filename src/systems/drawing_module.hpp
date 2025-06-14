#pragma once

#include <raylib.h>
#include <optional>
#include <entt/entt.hpp>
#include <bitset>
#include "../components/dot.hpp"
#include "../components/game_object.hpp"
#include "helpers/spell_utils.hpp"

void drawingModuleDotsCheck(GameObjects& game, entt::entity& drawingModule);
void renderDrawingModule(GameObjects& game, entt::entity& drawingModule);
entt::entity createDrawingModule(GameObjects& game);
void toggleDrawingModule(GameObjects& game, entt::entity& drawingModule);
std::optional<entt::entity> printAndResetConnections(GameObjects& game, entt::entity& drawingModule);