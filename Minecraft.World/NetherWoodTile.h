#pragma once
#include "Tile.h"

class ChunkRebuildData;
class NetherWoodTile : public Tile
{
	friend class ChunkRebuildData;
private:
	Icon *iconTop;

public:
	NetherWoodTile(int id);
public:
	virtual Icon *getTexture(int face, int data);
	virtual void playerWillDestroy(Level *level, int x, int y, int z, int data, shared_ptr<Player> player);
	void registerIcons(IconRegister *iconRegister);
};
