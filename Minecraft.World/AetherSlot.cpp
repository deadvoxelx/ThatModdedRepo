#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.inventory.h"
#include "net.minecraft.world.item.crafting.h"
#include "AetherSlot.h"

AetherSlot::AetherSlot(int slotNum, shared_ptr<Container> container, int id, int x, int y)
	: Slot( container, id, x, y ),
	slotNum( slotNum )
{
}

int AetherSlot::getMaxStackSize() const
{
	return 1;
}

bool AetherSlot::mayPlace(shared_ptr<ItemInstance> item)
{
	if (item == nullptr)
	{
		return false;
	}
	if (item->getItem()->id == Item::regenerationStone_Id)
	{
		return slotNum <= 2;
	}
	if (item->getItem()->id == Item::ironBubble_Id)
	{
		return slotNum <= 2;
	}
	if (item->getItem()->id == Item::ironPendant_Id || item->getItem()->id == Item::ironRing_Id)
	{
		return slotNum <= 2;
	}
	if (item->getItem()->id == Item::goldPendant_Id || item->getItem()->id == Item::goldRing_Id)
	{
		return slotNum <= 2;
	}
	if (item->getItem()->id == Item::zanitePendant_Id || item->getItem()->id == Item::zaniteRing_Id)
	{
		return slotNum <= 2;
	}
	if (item->getItem()->id == Item::agilityCape_Id || item->getItem()->id == Item::invisibilityCape_Id || item->getItem()->id == Item::swetCape_Id || item->getItem()->id == Item::valkyrieCape_Id)
	{
		return slotNum == 1;
	}
	return false;
}

Icon *AetherSlot::getNoItemIcon()
{
	return ArmorItem::getEmptyIcon(slotNum);
}