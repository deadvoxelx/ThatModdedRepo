#pragma once

class ItemInstance;
class Mob;

#include "Common/EventSystem/RubyEvent.h"

struct ItemInteractEntityEvent final : public RubyEvent {
    ItemInstance* item;
    Mob* mob;

    /* Ruby Launcher [IMPL-AT] (Minecraft.Client/PlayerConnection.cpp, InteractPacket) */
    ItemInteractEntityEvent(ItemInstance* item, Mob* mob) : item(item), mob(mob) {
        eventName = "ItemInteractEntity";
    }
};
