#pragma once
#include "Tile.h"

class ChunkRebuildData;
class NugrassTile : public Tile
{
	friend class ChunkRebuildData;

private:
	Icon *iconTop;
	Icon *iconBottom;

public:
	NugrassTile(int id);

	virtual Icon *getTexture(int face, int data);
	void registerIcons(IconRegister *iconRegister);
	virtual void playerDestroy(Level *level, shared_ptr<Player> player, int x, int y, int z, int data);

protected:
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);
};