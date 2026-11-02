#pragma once

#include "Item.h"
#include "ArmorItem.h"
#include "FoodItem.h"
#include "HoeItem.h"
#include "WeaponItem.h"
#include "PickaxeItem.h"
#include "HatchetItem.h"
#include "ShovelItem.h"

#include <string>

#include "sol/sol.hpp"

#include "ArmorMaterialRegistry.h"
#include "ItemDefinition.h"
#include "ItemTypes/ModArmorItem.h"
#include "ItemTypes/ModFoodItem.h"
#include "ItemTypes/ModHatchetItem.h"
#include "ItemTypes/ModHoeItem.h"
#include "ItemTypes/ModPickaxeItem.h"
#include "ItemTypes/ModShovelItem.h"
#include "ItemTypes/ModWeaponItem.h"
#include "ModItem.hpp"

class ItemFactory {
public:
    static Item* create(const ItemDefinition& def, int id) {
        switch (def.type) {
            case EBaseItem::Shovel:
                return (new ModShovelItem(id, def, def.tier));
            case EBaseItem::Hatchet:
                return (new ModHatchetItem(id,def, def.tier));
            case EBaseItem::Pickaxe:
                return (new ModPickaxeItem(id,def, def.tier));
            case EBaseItem::Weapon:
                return (new ModWeaponItem(id, def, def.tier));
            case EBaseItem::Hoe:
                return (new ModHoeItem(id, def, def.tier));
            case EBaseItem::Helmet:
            case EBaseItem::Chestplate:
            case EBaseItem::Leggings:
            case EBaseItem::Boots:
            {
                const ArmorItem::ArmorMaterial *material = ArmorMaterialRegistry::materialForIndex(def.armorMaterialIndex);
                if (material == nullptr) material = armorMaterialFor(def.armorMaterial);
                return (new ModArmorItem(id, material, def.armorModelIndex, armorSlotFor(def.type), def));
            }
            case EBaseItem::Food:
            case EBaseItem::Food_Fruit:
            case EBaseItem::Food_Bread:
            {
                ModFoodItem *food = new ModFoodItem(id, def, def.nutrition, def.saturationMod, def.isMeat);

                if (def.canAlwaysEat) food->setCanAlwaysEat();

                return food;
            }
            case EBaseItem::Default:
            default:
                return (new ModItem(id, def));
        }
    }
};