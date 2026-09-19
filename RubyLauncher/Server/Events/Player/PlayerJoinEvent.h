#pragma once

#include "ServerPlayer.h"

#include "Common/EventSystem/RubyEvent.h"

struct PlayerJoinEvent final : public RubyEvent {
    ServerPlayer* player;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/Network/PlayerList.cpp) */
    PlayerJoinEvent(ServerPlayer* player) : player(player) {
        eventName = "PlayerJoinEvent";
    }
};
