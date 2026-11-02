#pragma once

#include "../ItemDefinition.h"
#include "../ModItem.hpp"
#include "Item.h"
#include "ShovelItem.h"
#include "DiggerItem.h"
#include "../../../Lua/LuaStructs.h"

class ModShovelItem : public ModItemTemplate<ShovelItem> {
public:
    ModShovelItem(int id, const ItemDefinition &def, const Tier *tier): ModItemTemplate<ShovelItem>(id, def, tier) {};

    bool mineBlock(shared_ptr<ItemInstance> itemInstance, Level *level, int tile, int x, int y, int z, shared_ptr<LivingEntity> owner) override {
        if (itemDefinition.mineBlock.valid()) {
            auto result = itemDefinition.mineBlock(itemInstance, level, makeLuaBlock(LuaVec3(static_cast<double>(x), static_cast<double>(y), static_cast<double>(z)), tile, 0), owner);

            if (!result.valid()) {
                sol::error err = result;
                Loader::_debugPrint("Error calling mine block"+std::string(err.what()));
            }
            if (result.return_count() >= 1) return result.get<sol::optional<bool>>().value_or(false);
            return false;
        } else {
            return DiggerItem::mineBlock(itemInstance, level, tile, x, y, z, owner);
        }
    }

    bool isValidRepairItem(shared_ptr<ItemInstance> source, shared_ptr<ItemInstance> repairItem) override {
        if (itemDefinition.isValidRepairItem.valid()) {
            auto result = itemDefinition.isValidRepairItem(source, repairItem);

            if (!result.valid()) {
                sol::error err = result;
                Loader::_debugPrint("Error calling valid repair: "+std::string(err.what()));
            }
            if (result.return_count() >= 1) return result.get<sol::optional<bool>>().value_or(false);
            return false;
        } else {
            return ShovelItem::isValidRepairItem(source, repairItem);
        }
    }
};