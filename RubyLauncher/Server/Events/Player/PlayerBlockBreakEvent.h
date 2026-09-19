#pragma once

class ServerPlayer;
#include "Common/EventSystem/RubyEvent.h"
#include "Lua/LuaStructs.h"

struct PlayerBlockBreakEvent final : public CancellableRubyEvent {
    ServerPlayer* player;
    LuaBlock block;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/Player/ServerPlayerGameMode.cpp) */
    PlayerBlockBreakEvent(ServerPlayer* player, int x, int y, int z, int blockId, int blockAux) : player(player), block{LuaVec3(x,y,z), blockId, blockAux} {
        eventName = "PlayerBlockBreakEvent";
    }
};
