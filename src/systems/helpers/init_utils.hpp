#pragma once

#include <entt/entt.hpp>

#include "../../config.hpp"
#include "../../components/camera.hpp"
#include "../../components/player.hpp"
#include "../../components/game_object.hpp"

void initGameObjects(GameObjects& game);
entt::entity initPlayer(GameObjects& game);
entt::entity initCamera(GameObjects& game, Player player);