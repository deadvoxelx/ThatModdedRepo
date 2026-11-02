#pragma once

class ServerPlayer;
#include "Common/EventSystem/RubyEvent.h"
#include "Lua/LuaStructs.h"

struct PlayerHurtEvent final : public CancellableRubyEvent {
    ServerPlayer* player;
    ServerLevel* level;
    DamageSource* dmgSrc;
    int dmg;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/ServerPlayer.cpp) */
    PlayerHurtEvent(ServerPlayer* player, ServerLevel* level, DamageSource* src, int dmg) : player(player), level(level), dmgSrc(src), dmg(dmg) {
        eventName = "PlayerHurtEvent";
    }
};