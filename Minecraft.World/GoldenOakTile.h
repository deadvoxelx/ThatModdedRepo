#pragma once
#include "Tile.h"

class ChunkRebuildData;
class GoldenOakTile : public Tile
{
	friend class ChunkRebuildData;
private:
	Icon *iconTop;

public:
	GoldenOakTile(int id);

public:
	virtual Icon *getTexture(int face, int data);
	void registerIcons(IconRegister *iconRegister);
	virtual void playerDestroy(Level *level, shared_ptr<Player> player, int x, int y, int z, int data);

protected:
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);
};
