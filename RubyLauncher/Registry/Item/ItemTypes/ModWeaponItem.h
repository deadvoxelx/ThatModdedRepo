#pragma once

#include "../ItemDefinition.h"
#include "../ModItem.hpp"
#include "Item.h"
#include "WeaponItem.h"

class ModWeaponItem : public ModItemTemplate<WeaponItem> {
public:
    ModWeaponItem(int id, const ItemDefinition &def, const Tier *tier): ModItemTemplate<WeaponItem>(id, def, tier) {};

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
            return WeaponItem::isValidRepairItem(source, repairItem);
        }
    }
};