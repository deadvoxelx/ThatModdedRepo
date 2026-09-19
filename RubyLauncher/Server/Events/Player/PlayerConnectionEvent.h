#pragma once

class ServerPlayer;
#include "Common/EventSystem/RubyEvent.h"

struct PlayerConnectionEvent final : public RubyEvent {
    ServerPlayer* player;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/Network/PendingConnection.cpp) */
    PlayerConnectionEvent(ServerPlayer* player) : player(player) {
        eventName = "PlayerConnectionEvent";
    }
};
