#pragma once
#include "Tile.h"

class Player;

class QuicksoilTile : public Tile
{
	friend class Tile;

protected:
	QuicksoilTile(int id);

public:
	virtual void stepOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity);
};