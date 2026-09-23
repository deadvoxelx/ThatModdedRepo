#include "ItemRegistry.h"

#include "Item.h"

#include "Common/ModPaths.h"
#include "Host/RubyLauncherHost.h"
#include "ItemFactory.h"
#include "Loader.h"
#include "ModItem.h"
#include "Registry/IDs.h"
#include "TierRegistry.h"

std::map<int, std::wstring> ItemRegistry::langList;
int ItemRegistry::itemNameIdMax = 8191;	// Cranking this shit up so i dont gotta fw it again lmao
int ItemRegistry::itemIdMax = 8191;		// Changing this so mod items start at ID 8192
										// Im never adding that many items to the base lmao
namespace
{
	int useDescriptionFor(EBaseItem type)
	{
		switch (type)
		{
		case EBaseItem::Weapon:
			return IDS_DESC_SWORD;
		case EBaseItem::Pickaxe:
			return IDS_DESC_PICKAXE;
		case EBaseItem::Hatchet:
			return IDS_DESC_HATCHET;
		case EBaseItem::Shovel:
			return IDS_DESC_SHOVEL;
		case EBaseItem::Hoe:
			return IDS_DESC_HOE;
		case EBaseItem::Helmet:
			return IDS_DESC_HELMET;
		case EBaseItem::Chestplate:
			return IDS_DESC_CHESTPLATE;
		case EBaseItem::Leggings:
			return IDS_DESC_LEGGINGS;
		case EBaseItem::Boots:
			return IDS_DESC_BOOTS;
		case EBaseItem::Food:
			return IDS_DESC_APPLE;
		case EBaseItem::Food_Bread:
			return IDS_DESC_BREAD;
		case EBaseItem::Food_Fruit:
			return IDS_DESC_APPLE;
		case EBaseItem::Default:
		default:
			return IDS_DESC_STICK;
		}
	}

	RubyCreativeGroup creativeGroupFor(EBaseItem type)
	{
		switch (type)
		{
		case EBaseItem::Weapon:
		case EBaseItem::Hoe:
		case EBaseItem::Pickaxe:
		case EBaseItem::Hatchet:
		case EBaseItem::Shovel:
		case EBaseItem::Helmet:
		case EBaseItem::Chestplate:
		case EBaseItem::Leggings:
		case EBaseItem::Boots:
			return RubyCreativeGroup_Tools;
		case EBaseItem::Food:
		case EBaseItem::Food_Fruit:
		case EBaseItem::Food_Bread:
			return RubyCreativeGroup_Food;
		case EBaseItem::Default:
		default:
			return RubyCreativeGroup_Materials;
		}
	}

	int baseItemTypeFor(EBaseItem type)
	{
		switch (type)
		{
		case EBaseItem::Helmet:
			return Item::eBaseItemType_helmet;
		case EBaseItem::Chestplate:
			return Item::eBaseItemType_chestplate;
		case EBaseItem::Leggings:
			return Item::eBaseItemType_leggings;
		case EBaseItem::Boots:
			return Item::eBaseItemType_boots;
		default:
			return Item::eBaseItemType_undefined;
		}
	}

	int armorMaterialTypeFor(EArmorMaterial material)
	{
		switch (material)
		{
		case ArmorMaterial_Cloth:
			return Item::eMaterial_cloth;
		case ArmorMaterial_Chain:
			return Item::eMaterial_chain;
		case ArmorMaterial_Gold:
			return Item::eMaterial_gold;
		case ArmorMaterial_Diamond:
			return Item::eMaterial_diamond;
		case ArmorMaterial_Nethanium:
			return Item::eMaterial_nethanium;
		case ArmorMaterial_Endorium:
			return Item::eMaterial_endorium;
		case ArmorMaterial_Zanite:
			return Item::eMaterial_zanite;
		case ArmorMaterial_Gravitite:
			return Item::eMaterial_gravitite;
		case ArmorMaterial_Iron:
		default:
			return Item::eMaterial_iron;
		}
	}
}

int ItemRegistry::nextItemNameId() {
    itemNameIdMax += 1;
    return itemNameIdMax;
}

int ItemRegistry::nextItemId() {
    do {
        itemIdMax += 1;
    } while (itemIdMax < Item::ITEM_NUM_COUNT && (Item::items[itemIdMax] != nullptr));

    return itemIdMax;
}

int ItemRegistry::registerItem(const std::wstring& path, const std::string& id, const std::string& name, std::string modId, const ItemDefinition& def, const std::string& texturePath) {
    int nameId = nextItemNameId();
    int itemId = nextItemId();

    if (itemId >= Item::ITEM_NUM_COUNT) {
        Loader::_debugPrint("out of item ids, '" + id + "' was not registered");
        return -1;
    }

    std::wstring wname(name.begin(), name.end());

    const bool isArmor = armorSlotFor(def.type) >= 0;
    ItemDefinition resolved = def;

    if (isArmor)
    {
        resolved.armorModelIndex = RubyLoader::registerArmorSet(def.armorSet, RubyPaths::toNarrow(path), vanillaArmorModelIndex(def.armorMaterial));
    }

	if (resolved.tierIndex >= 0)
    {
        const Item::Tier *tier = TierRegistry::tierForIndex(resolved.tierIndex);
        if (tier != nullptr) resolved.tier = tier;
        else Loader::_debugPrint("item '" + id + "': unknown custom tier handle " + std::to_string(resolved.tierIndex));
    }

    Item *item = ItemFactory::create(resolved, itemId - 512);

    if (item == nullptr) {
        Loader::_debugPrint("could not create item '" + id + "'");
        return -1;
    }

    const std::wstring placeholderIconName = L"stick";
    std::wstring iconName = placeholderIconName;

    if (!texturePath.empty())
    {
        RubyModTexture request;
        request.iconName = rubyModTextureIconName(modId, id);
        request.filePath = RubyPaths::resolveModTexturePath(RubyPaths::toNarrow(path), texturePath);
        request.fallback = RubyPaths::toNarrow(placeholderIconName);
        request.block = false;

        RubyLoader::addModTexture(request);
        iconName = RubyPaths::toWide(request.iconName);
    }

    item->setIconName(iconName)->setDescriptionId(nameId)->setUseDescriptionId(useDescriptionFor(def.type));

    if (isArmor)
    {
        item->setBaseItemTypeAndMaterial(baseItemTypeFor(def.type), armorMaterialTypeFor(def.armorMaterial));
    }
    else if (def.type == EBaseItem::Food_Fruit || def.type == EBaseItem::Food_Bread)
    {
        const int material = (def.material != ItemMaterial_Undefined) ? itemMaterialFor(def.material) : (def.type == EBaseItem::Food_Bread ? Item::eMaterial_bread : Item::eMaterial_apple);
        item->setBaseItemTypeAndMaterial(craftingBaseItemTypeFor(def.type), material);
    }
    else if (isToolItem(def.type))
    {
        const int material = (def.material != ItemMaterial_Undefined) ? itemMaterialFor(def.material) : tierMaterialFor(def.tier);
        item->setBaseItemTypeAndMaterial(craftingBaseItemTypeFor(def.type), material);
    }
    else if (def.type == EBaseItem::Ingot)
    {
        const int material = (def.material != ItemMaterial_Undefined) ? itemMaterialFor(def.material) : Item::eMaterial_iron;
        item->setBaseItemTypeAndMaterial(Item::eBaseItemType_treasure, material);
    }
    else
    {
        item->handEquipped();

        if (def.type == EBaseItem::Default && def.material != ItemMaterial_Undefined)
        {
            item->setBaseItemTypeAndMaterial(Item::eBaseItemType_treasure, itemMaterialFor(def.material));
        }
    }

    if (def.isAccessory)
    {
        item->setAccessory();
    }

    if (def.maxStackSize)
	{
        item->setMaxStackSize(def.maxStackSize);
    }

	if (def.fireImmune)
    {
        item->setFireImmune();
    }

    if (def.blastImmune)
    {
        item->setBlastImmune();
    }

    langList[nameId] = wname;
    IDMapping::get()->add(modId, id, false, itemId);

    RubyCreative::addEntry(itemId, 0, creativeGroupFor(def.type));

    Loader::_debugPrint("registered item " + modId + ":" + id + " as id " + std::to_string(itemId) + (texturePath.empty() ? "" : ", texture '" + texturePath + "'"));

    return itemId;
}

void ItemRegistry::changeLang(StringTable& m_stringTable) {
    for (const auto& entry : langList) {
        m_stringTable.registerString(entry.first, entry.second);
    }
}
