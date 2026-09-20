#pragma once
#include <string>
#include "Common/EventSystem/RubyEvent.h"
class MinecraftServer;
struct ServerTickEvent : public RubyEvent {
    ServerTickEvent(MinecraftServer* server, int tickCount)
        : server(server), tickCount(tickCount)
    {
        eventName = "ServerTickEvent";
    }

    MinecraftServer* server;
    int tickCount;
};
