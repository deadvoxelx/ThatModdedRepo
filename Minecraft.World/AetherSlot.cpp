#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.inventory.h"
#include "net.minecraft.world.item.crafting.h"
#include "AetherSlot.h"

AetherSlot::AetherSlot(int slotNum, shared_ptr<Container> container, int id, int x, int y) : Slot( container, id, x, y ), slotNum( slotNum )
{
}

int AetherSlot::getMaxStackSize() const
{
	if (slotNum == 3) // Full stacks allowed in the offhand
	{
		return container->getMaxStackSize();
	}
	return 1;
}

bool AetherSlot::mayPlace(shared_ptr<ItemInstance> item)
{
	int id = item->getItem()->id;
	// Replacing the hardcoded defined items with the new isAccessory + isCape components
	bool isAccessory = (item->getItem()->isAccessory() || item->getItem()->isCape());

	if (item == nullptr)
	{
		return false;
	}

	// 1st 3 Slots are the actual Aether slots; 4th isnt used in the official Aether mod
	if (isAccessory)
	{
		if (item->getItem()->isCape())
		{
			return slotNum == 1;
		}
		return slotNum <= 2;
	}

	// 4th Slot acts as the "offhand" slot, everything but accessories can go here
	if (slotNum == 3)
	{
		return !isAccessory;
	}

	return false;
}

Icon *AetherSlot::getNoItemIcon()
{
	return ArmorItem::getEmptyIcon(slotNum);
}
