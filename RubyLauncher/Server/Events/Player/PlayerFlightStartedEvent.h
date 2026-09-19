#pragma once

class ServerPlayer;
#include "Common/EventSystem/RubyEvent.h"

struct PlayerFlightStartedEvent final : public RubyEvent {
    ServerPlayer* player;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/Network/PendingConnection.cpp) */
    PlayerFlightStartedEvent(ServerPlayer* player) : player(player) {
        eventName = "FlightStartedEvent";
    }
};
