#pragma once

#include "../../../../Minecraft.World/FoodItem.h"
#include "../ModItem.hpp"
#include "FoodItem.h"

class ModFoodItem : public ModItemTemplate<FoodItem>
{
public:
    ModFoodItem(int id, const ItemDefinition &def, int nutrition, float saturationMod, bool isMeat): ModItemTemplate<FoodItem>(id, def, nutrition, saturationMod, isMeat) {};

    void addEatEffect(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player) override {
        if (itemDefinition.eatEffect.valid()) {
            auto result = itemDefinition.eatEffect(instance, level, player);

            if (!result.valid()) {
                sol::error err = result;
                Loader::_debugPrint("Error calling eat effect: "+std::string(err.what()));
            }
        } else {
            FoodItem::addEatEffect(instance, level, player);
        }
    };
};

