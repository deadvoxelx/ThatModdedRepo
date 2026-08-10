#pragma once
// 4J Stu Added
// In InventoryMenu.java they create an anoymous class while creating some slot. I have moved the content
// of that anonymous class to here

#include "Slot.h"

class Container;

class AetherSlot : public Slot
{
private:
	const int slotNum;

public:
	AetherSlot(int slotNum, shared_ptr<Container> container, int id, int x, int y);
	virtual ~AetherSlot() {}

	virtual int getMaxStackSize() const;
	virtual bool mayPlace(shared_ptr<ItemInstance> item);
	Icon *getNoItemIcon();
};