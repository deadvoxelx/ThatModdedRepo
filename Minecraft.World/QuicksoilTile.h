#pragma once
#include "Tile.h"

class Player;

class QuicksoilTile : public Tile
{
	friend class Tile;

public:
	static const int TYPE_NATURAL = 0;
	static const int TYPE_UNNATURAL = 1;

protected:
	QuicksoilTile(int id);
	virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);

public:
	virtual void stepOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity);
	virtual void playerDestroy(Level *level, shared_ptr<Player> player, int x, int y, int z, int data);
};
