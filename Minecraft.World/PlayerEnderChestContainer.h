#pragma once

#include "SimpleContainer.h"

class EnderChestTileEntity;
class Level;

class PlayerEnderChestContainer : public SimpleContainer
{
private:
	shared_ptr<EnderChestTileEntity> activeChest;

public:
	PlayerEnderChestContainer();

	virtual int getContainerType();

	void setActiveChest(shared_ptr<EnderChestTileEntity> activeChest);
	void setItemsByTag(ListTag<CompoundTag> *enderItemsList, Level *level);	// Voxel - level-aware load (legacy save item id remap)
	ListTag<CompoundTag> *createTag();
	bool stillValid(shared_ptr<Player> player);
	void startOpen();
	void stopOpen();
	bool canPlaceItem(int slot, shared_ptr<ItemInstance> item);
};
