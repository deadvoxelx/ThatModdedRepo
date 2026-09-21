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

#include "ModItem.h"
#include "ArmorMaterialRegistry.h"

enum EBaseItem {
    Default,
    Food,
    Food_Fruit,
    Food_Bread,
    Hoe,
    Weapon,
    Pickaxe,
    Hatchet,
    Shovel,
    Helmet,
    Chestplate,
    Leggings,
    Boots,
    Ingot,
    Stick,
};

enum EItemMaterial {
    ItemMaterial_Undefined,
    ItemMaterial_Wood,
    ItemMaterial_Stone,
    ItemMaterial_Iron,
    ItemMaterial_Gold,
    ItemMaterial_Diamond,
    ItemMaterial_Cloth,
    ItemMaterial_Chain,
    ItemMaterial_Lapis,
    ItemMaterial_Redstone,
    ItemMaterial_Coal,
    ItemMaterial_Emerald,
    ItemMaterial_Quartz,
    ItemMaterial_Glass,
    ItemMaterial_Sand,
    ItemMaterial_Brick,
    ItemMaterial_Clay,
    ItemMaterial_Snow,
    ItemMaterial_Ice,
    ItemMaterial_Glowstone,
    ItemMaterial_Stick,
    ItemMaterial_Paper,
    ItemMaterial_Apple,
    ItemMaterial_Nethanium,
    ItemMaterial_Endorium,
    ItemMaterial_Zanite,
    ItemMaterial_Gravitite,
    ItemMaterial_Aphal,
    ItemMaterial_Nusa,
};

inline int itemMaterialFor(EItemMaterial material) {
    switch (material) {
        case ItemMaterial_Wood:      return Item::eMaterial_wood;
        case ItemMaterial_Stone:     return Item::eMaterial_stone;
        case ItemMaterial_Iron:      return Item::eMaterial_iron;
        case ItemMaterial_Gold:      return Item::eMaterial_gold;
        case ItemMaterial_Diamond:   return Item::eMaterial_diamond;
        case ItemMaterial_Cloth:     return Item::eMaterial_cloth;
        case ItemMaterial_Chain:     return Item::eMaterial_chain;
        case ItemMaterial_Lapis:     return Item::eMaterial_lapis;
        case ItemMaterial_Redstone:  return Item::eMaterial_redstone;
        case ItemMaterial_Coal:      return Item::eMaterial_coal;
        case ItemMaterial_Emerald:   return Item::eMaterial_emerald;
        case ItemMaterial_Quartz:    return Item::eMaterial_quartz;
        case ItemMaterial_Glass:     return Item::eMaterial_glass;
        case ItemMaterial_Sand:      return Item::eMaterial_sand;
        case ItemMaterial_Brick:     return Item::eMaterial_brick;
        case ItemMaterial_Clay:      return Item::eMaterial_clay;
        case ItemMaterial_Snow:      return Item::eMaterial_snow;
        case ItemMaterial_Ice:       return Item::eMaterial_ice;
        case ItemMaterial_Glowstone: return Item::eMaterial_glowstone;
        case ItemMaterial_Stick:     return Item::eMaterial_stick;
        case ItemMaterial_Paper:     return Item::eMaterial_paper;
        case ItemMaterial_Apple:     return Item::eMaterial_apple;
        case ItemMaterial_Nethanium: return Item::eMaterial_nethanium;
        case ItemMaterial_Endorium:  return Item::eMaterial_endorium;
        case ItemMaterial_Zanite:    return Item::eMaterial_zanite;
        case ItemMaterial_Gravitite: return Item::eMaterial_gravitite;
        case ItemMaterial_Aphal:     return Item::eMaterial_aphal;
        case ItemMaterial_Nusa:      return Item::eMaterial_nusa;
        case ItemMaterial_Undefined:
        default:                     return Item::eMaterial_undefined;
    }
}

inline int tierMaterialFor(const Item::Tier *tier) {
    if (tier == Item::Tier::WOOD)      return Item::eMaterial_wood;
    if (tier == Item::Tier::STONE)     return Item::eMaterial_stone;
    if (tier == Item::Tier::IRON)      return Item::eMaterial_iron;
    if (tier == Item::Tier::GOLD)      return Item::eMaterial_gold;
    if (tier == Item::Tier::DIAMOND)   return Item::eMaterial_diamond;
    if (tier == Item::Tier::NETHANIUM) return Item::eMaterial_nethanium;
    if (tier == Item::Tier::ENDORIUM)  return Item::eMaterial_endorium;
    if (tier == Item::Tier::ZANITE)    return Item::eMaterial_zanite;
    if (tier == Item::Tier::GRAVITITE) return Item::eMaterial_gravitite;
    if (tier == Item::Tier::VAMPIRE)   return Item::eMaterial_endorium;
    if (tier == Item::Tier::VALKYRIE)  return Item::eMaterial_iron;
    if (tier == Item::Tier::APHALAF)   return Item::eMaterial_aphal;
    if (tier == Item::Tier::NUSA)      return Item::eMaterial_nusa;
    return Item::eMaterial_undefined;
}

inline int craftingBaseItemTypeFor(EBaseItem type) {
    switch (type) {
        case EBaseItem::Weapon:     return Item::eBaseItemType_sword;
        case EBaseItem::Shovel:     return Item::eBaseItemType_shovel;
        case EBaseItem::Pickaxe:    return Item::eBaseItemType_pickaxe;
        case EBaseItem::Hatchet:    return Item::eBaseItemType_hatchet;
        case EBaseItem::Hoe:        return Item::eBaseItemType_hoe;
        case EBaseItem::Ingot:      return Item::eBaseItemType_treasure;
        case EBaseItem::Food_Fruit: return Item::eBaseItemType_giltFruit;
        case EBaseItem::Food_Bread: return Item::eBaseItemType_bread;
        case EBaseItem::Stick:      return Item::eBaseItemType_stick;
        default:                    return Item::eBaseItemType_undefined;
    }
}

inline bool isToolItem(EBaseItem type) {
    switch (type) {
        case EBaseItem::Weapon:
        case EBaseItem::Pickaxe:
        case EBaseItem::Hatchet:
        case EBaseItem::Shovel:
        case EBaseItem::Hoe:
            return true;
        default:
            return false;
    }
}

enum EItemTier {
    ItemTier_Wood,
    ItemTier_Stone,
    ItemTier_Iron,
    ItemTier_Diamond,
    ItemTier_Gold,
    ItemTier_Nethanium,
    ItemTier_Endorium,
    ItemTier_Zanite,
    ItemTier_Gravitite,
    ItemTier_Vampire,
    ItemTier_Valkyrie,
    ItemTier_Aphalaf,
    ItemTier_Nusa,
};

enum EArmorMaterial {
    ArmorMaterial_Cloth,
    ArmorMaterial_Chain,
    ArmorMaterial_Iron,
    ArmorMaterial_Gold,
    ArmorMaterial_Diamond,
    ArmorMaterial_Nethanium,
    ArmorMaterial_Endorium,
    ArmorMaterial_Zanite,
    ArmorMaterial_Gravitite,
};

inline const ArmorItem::ArmorMaterial* armorMaterialFor(EArmorMaterial material) {
    switch (material) {
        case ArmorMaterial_Cloth:     return ArmorItem::ArmorMaterial::CLOTH;
        case ArmorMaterial_Chain:     return ArmorItem::ArmorMaterial::CHAIN;
        case ArmorMaterial_Gold:      return ArmorItem::ArmorMaterial::GOLD;
        case ArmorMaterial_Diamond:   return ArmorItem::ArmorMaterial::DIAMOND;
        case ArmorMaterial_Nethanium: return ArmorItem::ArmorMaterial::NETHANIUM;
        case ArmorMaterial_Endorium:  return ArmorItem::ArmorMaterial::ENDORIUM;
        case ArmorMaterial_Zanite:    return ArmorItem::ArmorMaterial::ZANITE;
        case ArmorMaterial_Gravitite: return ArmorItem::ArmorMaterial::GRAVITITE;
        case ArmorMaterial_Iron:
        default:                      return ArmorItem::ArmorMaterial::IRON;
    }
}

inline int vanillaArmorModelIndex(EArmorMaterial material) {
    switch (material) {
        case ArmorMaterial_Cloth:     return 0;
        case ArmorMaterial_Chain:     return 1;
        case ArmorMaterial_Diamond:   return 3;
        case ArmorMaterial_Gold:      return 4;
        case ArmorMaterial_Nethanium: return 5;
        case ArmorMaterial_Endorium:  return 6;
        case ArmorMaterial_Zanite:    return 7;
        case ArmorMaterial_Gravitite: return 8;
        case ArmorMaterial_Iron:
        default:                      return 2;
    }
}

inline int armorSlotFor(EBaseItem type) {
    switch (type) {
        case EBaseItem::Helmet:     return ArmorItem::SLOT_HEAD;
        case EBaseItem::Chestplate: return ArmorItem::SLOT_TORSO;
        case EBaseItem::Leggings:   return ArmorItem::SLOT_LEGS;
        case EBaseItem::Boots:      return ArmorItem::SLOT_FEET;
        default:                    return -1;
    }
}

inline const Item::Tier* tierFor(EItemTier tier) {
    switch (tier) {
        case ItemTier_Stone:     return Item::Tier::STONE;
        case ItemTier_Iron:      return Item::Tier::IRON;
        case ItemTier_Diamond:   return Item::Tier::DIAMOND;
        case ItemTier_Gold:      return Item::Tier::GOLD;
        case ItemTier_Nethanium: return Item::Tier::NETHANIUM;
        case ItemTier_Endorium:  return Item::Tier::ENDORIUM;
        case ItemTier_Zanite:    return Item::Tier::ZANITE;
        case ItemTier_Gravitite: return Item::Tier::GRAVITITE;
        case ItemTier_Vampire:   return Item::Tier::VAMPIRE;
        case ItemTier_Valkyrie:  return Item::Tier::VALKYRIE;
        case ItemTier_Aphalaf:   return Item::Tier::APHALAF;
        case ItemTier_Nusa:      return Item::Tier::NUSA;
        case ItemTier_Wood:
        default:                 return Item::Tier::WOOD;
    }
}

struct ItemDefinition {
    EBaseItem type;

    // Food Items
    int nutrition = 0;
    float saturationMod = 0.0;
    bool isMeat = false;
    bool canAlwaysEat = false;

    // Tool Items
    const Item::Tier* tier = Item::Tier::WOOD;
    int tierIndex = -1;
    bool isAccessory = false;
    int maxStackSize = 64;

    // Armor Items
    EArmorMaterial armorMaterial = ArmorMaterial_Iron;
    std::string armorSet = "";
    int armorModelIndex = -1;
    int armorMaterialIndex = -1;

    EItemMaterial material = ItemMaterial_Undefined;

    ItemDefinition(sol::table items) {
        type = items["base"].get<EBaseItem>();
        sol::optional<int> n = items["nutrition"];
        sol::optional<float> s = items["saturationMod"];
        sol::optional<bool> meat = items["isMeat"];
        sol::optional<bool> accessory = items["isAccessory"];
        sol::optional<bool> alwaysEat = items["canAlwaysEat"];
        sol::optional<Item::Tier*> t = items["tier"];
        sol::optional<std::string> armor = items["armorSet"];
        sol::optional<int> customTier = items["customTier"];
        sol::optional<int> customArmor = items["customArmorMaterial"];
        sol::optional<int> stackSize = items["maxStacksize"];

        if (items["material"].is<int>()) {
            material = items["material"].get<EItemMaterial>();
        }

        if (n) nutrition = n.value();
        if (s) saturationMod = s.value();
        if (meat) isMeat = meat.value();
        if (accessory) isAccessory = accessory.value();
        if (stackSize) maxStackSize = stackSize.value();
        if (alwaysEat) canAlwaysEat = alwaysEat.value();
        if (armor) armorSet = armor.value();
        if (customTier) tierIndex = customTier.value();
        if (customArmor) armorMaterialIndex = customArmor.value();

        if (items["armorMaterial"].is<int>()) {
            armorMaterial = items["armorMaterial"].get<EArmorMaterial>();
        }

        if (t) {
            tier = t.value();
        } else if (items["tier"].is<int>()) {
            tier = tierFor(items["tier"].get<EItemTier>());
        }

        if (armorModelIndex < 0) armorModelIndex = vanillaArmorModelIndex(armorMaterial);
    }
};

class ItemFactory {
public:
    static Item* create(const ItemDefinition& def, int id) {
        switch (def.type) {
            case EBaseItem::Shovel:
                return (new ShovelItem(id,def.tier));
            case EBaseItem::Hatchet:
                return (new HatchetItem(id,def.tier));
            case EBaseItem::Pickaxe:
                return (new PickaxeItem(id,def.tier));
            case EBaseItem::Weapon:
                return (new WeaponItem(id,def.tier));
            case EBaseItem::Hoe:
                return (new HoeItem(id,def.tier));
            case EBaseItem::Helmet:
            case EBaseItem::Chestplate:
            case EBaseItem::Leggings:
            case EBaseItem::Boots:
            {
                const ArmorItem::ArmorMaterial *material = ArmorMaterialRegistry::materialForIndex(def.armorMaterialIndex);
                if (material == nullptr) material = armorMaterialFor(def.armorMaterial);
                return (new ArmorItem(id, material, def.armorModelIndex, armorSlotFor(def.type)));
            }
            case EBaseItem::Food:
            case EBaseItem::Food_Fruit:
            case EBaseItem::Food_Bread:
            {
                FoodItem *food = new FoodItem(id,def.nutrition,def.saturationMod,def.isMeat);

                if (def.canAlwaysEat) food->setCanAlwaysEat();

                return food;
            }
            case EBaseItem::Default:
            default:
                return (new ModItem(id));
        }
    }
};