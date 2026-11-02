#pragma once

class ServerLevel;
class ServerPlayer;

#include "Common/EventSystem/RubyEvent.h"

struct BlockUseEvent final : public RubyEvent {
    LuaBlock block;
    ServerLevel* level;
    ServerPlayer* player;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/Tile.cpp) */
    BlockUseEvent(int x, int y, int z, int blockId, ServerLevel* level, ServerPlayer* player) : block{LuaVec3(x,y,z), blockId}, level(level), player(player)  {
        eventName = "BlockUseEvent";
    }
};
