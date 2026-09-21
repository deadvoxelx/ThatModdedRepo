#pragma once

class ItemInstance;
class ServerLevel;
#include "ServerPlayer.h"

#include "Common/EventSystem/RubyEvent.h"

struct ItemInteractEvent final : public RubyEvent {
    ItemInstance* item;
    ServerLevel* level;
    ServerPlayer* player;

    /* Ruby Launcher [IMPL-AT] (Minecraft.World/Item/Item.cpp) */
    ItemInteractEvent(ItemInstance* item, ServerLevel* level, ServerPlayer* player) : item(item), level(level), player(player) {
        eventName = "PlayerItemInteractEvent";
    }
};
