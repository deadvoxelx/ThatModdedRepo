#pragma once

#include <utility>

class ServerLevel;
#include "ServerPlayer.h"

#include "Common/EventSystem/RubyEvent.h"

struct ItemCompleteUseEvent final : public RubyEvent {
    ItemInstance* item;
    ServerLevel* level;
    ServerPlayer* player;
    std::string itemId;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/ServerPlayer.cpp, completeUsingItem) */
    ItemCompleteUseEvent(ItemInstance* item, ServerLevel* level, ServerPlayer* player, std::string itemId) : item(item), level(level), player(player), itemId(std::move(itemId)) {
        eventName = "ItemCompleteUseEvent";
    }
};
