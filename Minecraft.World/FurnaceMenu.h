#pragma once

#include "AbstractContainerMenu.h"
#include "Slot.h"

class FurnaceTileEntity;

class FurnaceMenu : public AbstractContainerMenu
{
private:
	class FuelSlot : public Slot
	{
	public:
		FuelSlot(shared_ptr<Container> container, int slot, int x, int y);

		bool mayPlace(shared_ptr<ItemInstance> item);
	};

public:
	static const int INGREDIENT_SLOT = 0;
	static const int FUEL_SLOT = 1;
	static const int RESULT_SLOT = 2;
	static const int INV_SLOT_START = FurnaceMenu::RESULT_SLOT + 1;
	static const int INV_SLOT_END = FurnaceMenu::INV_SLOT_START + 9 * 3;
	static const int USE_ROW_SLOT_START = FurnaceMenu::INV_SLOT_END;
	static const int USE_ROW_SLOT_END = FurnaceMenu::USE_ROW_SLOT_START + 9;

private:
	shared_ptr<FurnaceTileEntity> furnace;

public:
	FurnaceMenu(shared_ptr<Inventory> inventory, shared_ptr<FurnaceTileEntity> furnace);

	shared_ptr<FurnaceTileEntity> getFurnace() { return furnace; }

private:
	int tc;
	int lt;
	int ld;

public:
	virtual void addSlotListener(ContainerListener *listener);
	virtual void broadcastChanges();
	virtual void setData(int id, int value);
	virtual bool stillValid(shared_ptr<Player> player);
	virtual shared_ptr<ItemInstance> quickMoveStack(shared_ptr<Player> player, int slotIndex);

	virtual shared_ptr<ItemInstance> clicked(int slotIndex, int buttonNum, int clickType, shared_ptr<Player> player, bool looped = false);
};
