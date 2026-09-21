#pragma once

class ItemInstance;
class ServerPlayer;
class SerevrLevel;

#include "Common/EventSystem/RubyEvent.h"

struct ItemTickEvent final : public RubyEvent {
    ItemInstance* item;
    ServerLevel* level;
    ServerPlayer* player;
    int slot;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/PlayerConnection.cpp, InteractPacket) */
    ItemTickEvent(ItemInstance* item, ServerLevel* level, ServerPlayer* player, int slot) : item(item), level(level), player(player), slot(slot) {
        eventName = "ItemTickEvent";
    }
};
