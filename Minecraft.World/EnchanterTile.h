#pragma once
#include "BaseEntityTile.h"

class Mob;
class Player;
class Random;
class ChunkRebuildData;

class EnchanterTile : public BaseEntityTile
{
	friend class Tile;
	friend class ChunkRebuildData;

private:
	Random *random;
	static bool noDrop;
	Icon *iconTop;

public:
	EnchanterTile(int id);
	virtual void onPlace(Level *level, int x, int y, int z);
	virtual bool TestUse();
	virtual bool use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly = false);
	virtual void setPlacedBy(Level *level, int x, int y, int z, shared_ptr<LivingEntity> by, shared_ptr<ItemInstance> itemInstance);
	virtual void onRemove(Level *level, int x, int y, int z, int id, int data);
	virtual bool hasAnalogOutputSignal();
	virtual int getAnalogOutputSignal(Level *level, int x, int y, int z, int dir);
	virtual int cloneTileId(Level *level, int x, int y, int z);

	virtual Icon *getTexture(int face, int data);
	void registerIcons(IconRegister *iconRegister);

protected:
	virtual shared_ptr<TileEntity> newTileEntity(Level *level);
};