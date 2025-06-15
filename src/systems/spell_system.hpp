#pragma once

#include <raylib.h>
#include <entt/entt.hpp>
#include <optional>
#include "../config.hpp"
#include "../components/spell.hpp"
#include "../components/game_object.hpp"
#include "../components/dot.hpp"
#include "../components/enemy.hpp"
#include "helpers/rendering_utils.hpp"
#include "helpers/spell_utils.hpp"
#include "helpers/spell_damage_check.hpp"

// draw spells
void drawSpells(GameObjects& game, Vector2 playerPosScreen);
void drawIsoTriangle(Vector2 start, Vector2 end, float size, Color color);
void drawIsoRectangle(Vector2 center, float width, float height, Color color);
void drawIsoLine(Vector2 start, Vector2 end, float thickness, Color color);
void drawSpellRangeCircle(GameObjects& game, Vector2 playerPos, std::optional<entt::entity> spell, Color color);
void drawIsoCircle(Vector2 center, float radius, Color color, const bool fill = true);
void writeCastedSpell(GameObjects& game, std::optional<entt::entity> spellEntity);

// spell logic
std::optional<Vector2> handleSpellTargeting(GameObjects& game, Camera2D camera, std::optional<entt::entity> spell, entt::entity& drawingModule, Vector2 playerIsoPos);
void hitEnemy(GameObjects& game, SpellActiveComponent& activeComponent, SpellShape& shape);
void activeSpellHandler(GameObjects& game, std::optional<entt::entity>& activeSpell, std::optional<Vector2>& spellTarget, std::optional<entt::entity>& spellCasted, Vector2 playerPos);