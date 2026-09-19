#pragma once

class ServerPlayer;
#include "Common/EventSystem/RubyEvent.h"

struct PlayerFlightEndedEvent final : public RubyEvent {
    ServerPlayer* player;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/Network/PendingConnection.cpp) */
    PlayerFlightEndedEvent(ServerPlayer* player) : player(player) {
        eventName = "FlightEndedEvent";
    }
};
