#pragma once

#include "WaterLilyTileItem.h"
#include "TilePlanterItem.h"
#include "SkullItem.h"
#include "TileItem.h"
#include "SignItem.h"
#include "DoorItem.h"
#include "BedItem.h"

class Item;

namespace RubyUtils {
    static bool isPlaceableItem(Item* item) {
        if (item == nullptr) return false;
        return dynamic_cast<TileItem*>(item) != nullptr ||
               dynamic_cast<TilePlanterItem*>(item) != nullptr ||
               dynamic_cast<SkullItem*>(item) != nullptr ||
               dynamic_cast<WaterLilyTileItem*>(item) != nullptr ||
               dynamic_cast<SignItem*>(item) != nullptr ||
               dynamic_cast<DoorItem*>(item) != nullptr ||
               dynamic_cast<BedItem*>(item) != nullptr;
    }

    static void LuaException(lua_State* L, const std::string& message) {
        lua_Debug info;
        lua_getstack(L, 1, &info);
        lua_getinfo(L, "Sl", &info);
        Loader::_debugPrint(message + " (" + info.short_src + ":" + std::to_string(info.currentline) + ")");
    }
}