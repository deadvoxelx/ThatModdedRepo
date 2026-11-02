#pragma once

class ServerLevel;

#include "Common/EventSystem/RubyEvent.h"

struct BlockTickEvent final : public RubyEvent {
    LuaBlock block;
    ServerLevel* level;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/Tile.cpp) */
    BlockTickEvent(int x, int y, int z, int blockId, ServerLevel* level) : block{LuaVec3(x,y,z), blockId}, level(level)  {
        eventName = "BlockTickEvent";
    }
};
