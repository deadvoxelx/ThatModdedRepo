#pragma once

#include "../ItemDefinition.h"
#include "../ModItem.hpp"
#include "ArmorItem.h"

class ModArmorItem : public ModItemTemplate<ArmorItem> {
public:
    ModArmorItem(int id, const ArmorItem::ArmorMaterial *armorType, int icon, int slot, const ItemDefinition &def): ModItemTemplate<ArmorItem>(id, def, armorType, icon, slot) {};

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
            return ArmorItem::isValidRepairItem(source, repairItem);
        }
    }
};
