#pragma once

class ServerPlayer;
#include "Common/EventSystem/RubyEvent.h"
#include "Lua/LuaStructs.h"

struct PlayerBlockPlaceEvent final : public CancellableRubyEvent {
    ServerPlayer* player;
    LuaBlock block;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/Network/PlayerConnection.cpp) */
    PlayerBlockPlaceEvent(ServerPlayer* player, int x, int y, int z, int blockId) : player(player), block{LuaVec3(x,y,z), blockId} {
        eventName = "PlayerBlockPlaceEvent";
    }
};